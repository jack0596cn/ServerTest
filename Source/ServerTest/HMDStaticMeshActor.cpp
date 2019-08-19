// Fill out your copyright notice in the Description page of Project Settings.

#include "HMDStaticMeshActor.h"
#include "TimerManager.h"
#include "UnrealNetwork.h"

AHMDStaticMeshActor::AHMDStaticMeshActor()
{
	CountDownTimer = 10;

	SMCom = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("HMDMeshCom"));
	SMCom->SetupAttachment(RootComponent);

	TRCCom = CreateDefaultSubobject<UTextRenderComponent>(TEXT("TextRenderCom"));
	TRCCom->SetupAttachment(RootComponent);

	SetMobility(EComponentMobility::Movable);
	//every frame tick
	PrimaryActorTick.bCanEverTick = true;
}

void AHMDStaticMeshActor::BeginPlay()
{	
	if (SMCom)
	{
		FString SMPath = TEXT("/Game/Mesh/GenericHMD.GenericHMD");
		UStaticMesh* SM = LoadObject<UStaticMesh>(nullptr, *SMPath, nullptr, LOAD_None, nullptr);
		SMCom->SetStaticMesh(SM);
	}

	if (TRCCom)
	{
		TRCCom->SetText(FText::FromString(TEXT("Default")));
	}

	if (Role == ROLE_Authority)
	{
		FTimerDelegate TimerDelegate = FTimerDelegate::CreateUObject(this, &AHMDStaticMeshActor::OneSecTick);
		GetWorldTimerManager().SetTimer(OneSecTimerHandle, TimerDelegate, 1.0f, true);	
	}

	UE_LOG(LogTemp, Log, TEXT("BeginPlay"));
	
	Super::BeginPlay();
}

void AHMDStaticMeshActor::OneSecTick()
{
	if (CountDownTimer > 0)
	{
		CountDownTimer -= 1;
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("OneSecTick"));
	}

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::FromInt(CountDownTimer));
	
}

void AHMDStaticMeshActor::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	//Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	//DOREPLIFETIME(AHMDStaticMeshActor, CountDownTimer);
}

void AHMDStaticMeshActor::Tick(float DeltaSeconds)
{
	if (TRCCom)
	{
		TRCCom->SetText(FText::FromString(FString::FromInt(CountDownTimer)));
	}

	Super::Tick(DeltaSeconds);
}



