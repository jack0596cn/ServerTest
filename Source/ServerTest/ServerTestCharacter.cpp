// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "ServerTestCharacter.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "HMDStaticMeshActor.h"
#include "GameFramework/PlayerInput.h"
#include "ReceiveProcess.h"
#include "MyGameInstance.h"
#include "UIManager.h"

#define LOCTEXT_NAMESPACE "ServerTest"

//#define SMBP

//////////////////////////////////////////////////////////////////////////
// AServerTestCharacter

AServerTestCharacter::AServerTestCharacter()
{
	Socket = nullptr;

	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)

	MoveMeshTestCom = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("HMDMeshCom"));
	MoveMeshTestCom->SetMobility(EComponentMobility::Movable);
	MoveMeshTestCom->SetIsReplicated(true);

	//MoveMeshTestCom->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
	//MoveMeshTestCom->RegisterComponent();
	//MoveMeshTestCom->SetRelativeLocation(FVector(10, 0, 90));
}

void AServerTestCharacter::BeginPlay()
{
#ifdef SMBP
	// 	FString SuffixStr = TEXT("_C");
// 	FString BPPath = TEXT("/Game/BP/SM_Mesh_ProgramUse.SM_Mesh_ProgramUse") + SuffixStr;
// 	TSubclassOf<AHMDStaticMeshActor> SMA = LoadClass<AHMDStaticMeshActor>(nullptr, *BPPath, nullptr);
//	
// 	if (SMA)
// 	{
// 		SMActor = GetWorld()->SpawnActor<AHMDStaticMeshActor>(SMA, FVector(-420.000000, 410.000000, 220.000000), FRotator(0, 180, 0));
// 		if (SMActor)
// 		{
// 			SMActor->SetMobility(EComponentMobility::Movable);
// 		}
// 		
// 		UE_LOG(LogTemp, Log, TEXT("BeginPlay"));
// 	}
#else
// 	SMAPtr = GetWorld()->SpawnActor<AHMDStaticMeshActor>(AHMDStaticMeshActor::StaticClass(), \
// 		FVector(-420.000000, 410.000000, 220.000000), FRotator(0, 180, 0));
// 
// 	if (SMAPtr)
// 	{
// 		UE_LOG(LogTemp, Log, TEXT("BeginPlay"));
// 	}

	UStaticMesh* SM = LoadObject<UStaticMesh>(nullptr,TEXT("/Game/Mesh/GenericHMD.GenericHMD"), nullptr, LOAD_None, nullptr);
	if (SM)
	{
		MoveMeshTestCom->SetStaticMesh(SM);
	}

#endif // SMBP

	ListenOut();

	LinkOuterServer();

	UUIManager::GetInstance()->GrantItems();

	Super::BeginPlay();
}

void AServerTestCharacter::Tick(float DeltaSeconds)
{
	uint8 Buffer[2 * 1024] = { 0 };
	int32 DataLen = 0;
	if (Socket && Socket->Recv(Buffer, sizeof(Buffer) - 1, DataLen, ESocketReceiveFlags::None))
	{
		FUTF8ToTCHAR WideBuffer(reinterpret_cast<const ANSICHAR*>(Buffer));
		TCHAR* RecStr = (TCHAR*)WideBuffer.Get();
		
		FString Temp = RecStr;

		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, *Temp);
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::FromInt(Role));

		if (Temp == TEXT("AAA_Resp"))
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("����"));
			JumpOnServer();
		}
	}

	Super::Tick(DeltaSeconds);
}

//////////////////////////////////////////////////////////////////////////
// Input

void AServerTestCharacter::InitializeInputBindings()
{
	static bool bBindingsAdded = false;
	if (!bBindingsAdded)
	{
		bBindingsAdded = true;
		UPlayerInput::AddEngineDefinedActionMapping(FInputActionKeyMapping("GPress", EKeys::G));
		UPlayerInput::AddEngineDefinedActionMapping(FInputActionKeyMapping("HPress", EKeys::H));
		UPlayerInput::AddEngineDefinedActionMapping(FInputActionKeyMapping("JPress", EKeys::J));
	}
}

void AServerTestCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	
	InitializeInputBindings();

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAxis("MoveForward", this, &AServerTestCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AServerTestCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &AServerTestCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AServerTestCharacter::LookUpAtRate);

	// handle touch devices
	PlayerInputComponent->BindTouch(IE_Pressed, this, &AServerTestCharacter::TouchStarted);
	PlayerInputComponent->BindTouch(IE_Released, this, &AServerTestCharacter::TouchStopped);

	// VR headset functionality
	PlayerInputComponent->BindAction("ResetVR", IE_Pressed, this, &AServerTestCharacter::OnResetVR);

	PlayerInputComponent->BindAction("GPress", IE_Pressed, this, &AServerTestCharacter::OnGPress);
	PlayerInputComponent->BindAction("HPress", IE_Pressed, this, &AServerTestCharacter::OnHPress);
	PlayerInputComponent->BindAction("JPress", IE_Pressed, this, &AServerTestCharacter::OnJPress);
}


bool AServerTestCharacter::JumpOnServer_Validate()
{
	return true;
}

void AServerTestCharacter::JumpOnServer_Implementation()
{
	Jump();
}

void AServerTestCharacter::OnJPress()
{
	OutServerCMD();
}

bool AServerTestCharacter::OutServerCMD_Validate()
{
	return true;
}

void AServerTestCharacter::OutServerCMD_Implementation()
{
	if (HasAuthority())
	{
		Socket = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateSocket(NAME_Stream, TEXT("default"), false);

		FString address = TEXT("127.0.0.1");
		int32 port = 8888;
		FIPv4Address ip;
		FIPv4Address::Parse(address, ip);

		TSharedRef<FInternetAddr> addr = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr();
		addr->SetIp(ip.Value);
		addr->SetPort(port);

		bool connected = Socket->Connect(*addr);
		if (connected)
		{
			FString serialized = TEXT("AAA");
			TCHAR *serializedChar = serialized.GetCharArray().GetData();
			int32 size = FCString::Strlen(serializedChar);
			int32 sent = 0;

			if (Socket != nullptr)
			{
				bool successful = Socket->Send((uint8*)TCHAR_TO_UTF8(serializedChar), size, sent);
				if (successful)
				{
					UE_LOG(LogTemp, Log, TEXT("send %s success!!"), *serialized);
				}
			}
		}
		/*
		FString serialized = TEXT("AAA");
		TCHAR *serializedChar = serialized.GetCharArray().GetData();
		int32 size = FCString::Strlen(serializedChar);
		int32 sent = 0;

		UMyGameInstance* myGI = Cast<UMyGameInstance>(GetGameInstance());
		FSocket *Socket = myGI->GetSocket();

		bool successful = Socket->Send((uint8*)TCHAR_TO_UTF8(serializedChar), size, sent);
		if (successful)
		{
			UE_LOG(LogTemp, Log, TEXT("send %s success!!"), *serialized);
		}
		*/
	}
}

void AServerTestCharacter::OnGPress()
{
	Mount();
}

bool AServerTestCharacter::Mount_Validate()
{
	return true;
}

void AServerTestCharacter::Mount_Implementation()
{
	Mount_MulticastRPCFunction();
}

void AServerTestCharacter::OnHPress()
{
	Demount();
}

bool AServerTestCharacter::Demount_Validate()
{
	return true;
}

void AServerTestCharacter::Demount_Implementation()
{
	Demount_MulticastRPCFunction();
}

bool AServerTestCharacter::ListenOut_Validate()
{
	return true;
}

void AServerTestCharacter::ListenOut_Implementation()
{
	FVector Location(-420.000000, 410.000000, 220.000000);
	AReceiveProcess* RP = GetWorld()->SpawnActor<AReceiveProcess>(AReceiveProcess::StaticClass(), \
		Location, FRotator(0, 180, 0));
}

void AServerTestCharacter::Mount_MulticastRPCFunction_Implementation()
{
	if (MoveMeshTestCom->IsValidLowLevel())
	{
		if (Role < ROLE_Authority)
		{
			MoveMeshTestCom->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
			//MoveMeshTestCom->RegisterComponent();
			MoveMeshTestCom->SetRelativeLocation(FVector(10, 0, 90));
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::FromInt(Role));
		}
	}
}

bool AServerTestCharacter::Mount_MulticastRPCFunction_Validate()
{
	return true;
}

void AServerTestCharacter::Demount_MulticastRPCFunction_Implementation()
{
	if (MoveMeshTestCom->IsValidLowLevel())
	{
		if (Role < ROLE_Authority)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::FromInt(Role));
			FDetachmentTransformRules detachRule(EDetachmentRule::KeepRelative, \
				EDetachmentRule::KeepRelative, \
				EDetachmentRule::KeepRelative, \
				true);
			MoveMeshTestCom->DetachFromComponent(detachRule);
		}
	}
}

bool AServerTestCharacter::Demount_MulticastRPCFunction_Validate()
{
	return true;
}

bool AServerTestCharacter::LinkOuterServer_Validate()
{
	return true;
}

void AServerTestCharacter::LinkOuterServer_Implementation()
{
	if (Role == ROLE_Authority) 
	{
		Socket = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateSocket(NAME_Stream, TEXT("default"), false);
		
		FString address = TEXT("127.0.0.1");
		int32 port = 8888;
		FIPv4Address ip;
		FIPv4Address::Parse(address, ip);

		TSharedRef<FInternetAddr> addr = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr();
		addr->SetIp(ip.Value);
		addr->SetPort(port);

		bool connected = Socket->Connect(*addr);
		if (connected)
		{
			FString serialized = TEXT("loadPlayer|1");
			TCHAR *serializedChar = serialized.GetCharArray().GetData();
			int32 size = FCString::Strlen(serializedChar);
			int32 sent = 0;

			bool successful = Socket->Send((uint8*)TCHAR_TO_UTF8(serializedChar), size, sent);
			if (successful)
			{
				UE_LOG(LogTemp, Log, TEXT("send success!!"));
			}
		}
		/*
		FString serialized = TEXT("loadPlayer|1");
		TCHAR *serializedChar = serialized.GetCharArray().GetData();
		int32 size = FCString::Strlen(serializedChar);
		int32 sent = 0;

		//FSocket *Socket = UMyGameInstance::GetSocket();
		UMyGameInstance* myGI = Cast<UMyGameInstance>(GetGameInstance());
		FSocket *Socket = myGI->GetSocket();
		bool successful = Socket->Send((uint8*)TCHAR_TO_UTF8(serializedChar), size, sent);
		if (successful)
		{
			UE_LOG(LogTemp, Log, TEXT("send success!!"));
		}
		*/
	}
}

void AServerTestCharacter::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	//DOREPLIFETIME(AServerTestCharacter, MoveMeshTestCom);	
	//DOREPLIFETIME_CONDITION(AServerTestCharacter, CountDownTimer, COND_OwnerOnly)
}

void AServerTestCharacter::OnResetVR()
{
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

void AServerTestCharacter::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
	Jump();
}

void AServerTestCharacter::TouchStopped(ETouchIndex::Type FingerIndex, FVector Location)
{
	StopJumping();
}

void AServerTestCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AServerTestCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void AServerTestCharacter::MoveForward(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void AServerTestCharacter::MoveRight(float Value)
{
	if ( (Controller != NULL) && (Value != 0.0f) )
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
	
		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}

void AServerTestCharacter::MyPrint()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Exec CMD"));
}

#undef LOCTEXT_NAMESPACE