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

	bNetLoadOnClient = true;

	SetReplicates(true);

	TRCCom->SetText(FText::FromString(TEXT("")));

}

void AHMDStaticMeshActor::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

void AHMDStaticMeshActor::BeginPlay()
{	
	if (SMCom)
	{
		FString SMPath = TEXT("/Game/Mesh/GenericHMD.GenericHMD");
		UStaticMesh* SM = LoadObject<UStaticMesh>(nullptr, *SMPath, nullptr, LOAD_None, nullptr);
		SMCom->SetStaticMesh(SM);
	}

	bool hasAuthority = HasAuthority();
	if (hasAuthority)
	{
		GetWorldTimerManager().SetTimer(OneSecTimerHandle, this, &AHMDStaticMeshActor::OneSecTick, 1.0f, true);
	}

	UE_LOG(LogTemp, Log, TEXT("BeginPlay"));

	Super::BeginPlay();
}

void AHMDStaticMeshActor::OneSecTick()
{
	if (CountDownTimer > 0)
	{
		CountDownTimer = CountDownTimer - 1;
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("OneSecTick"));
	}
	
	TRCCom->SetText(FText::FromString(FString::FromInt(CountDownTimer)));
}

void AHMDStaticMeshActor::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(AHMDStaticMeshActor, CountDownTimer, COND_OwnerOnly)
}

void AHMDStaticMeshActor::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}



