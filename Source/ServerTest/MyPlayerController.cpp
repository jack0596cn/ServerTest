// Fill out your copyright notice in the Description page of Project Settings.

#include "MyPlayerController.h"
#include "UIManager.h"
#include "TcpClient.h"
#include "EngineUtils.h"
#include "ServerTestCharacter.h"

AMyPlayerController::AMyPlayerController(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void AMyPlayerController::BeginPlay()
{
	Super::BeginPlay();

	bCanRun = false;
	bTalk = false;
	DialogString = "";

	TSharedRef<SWidget> MyUI = UUIManager::GetInstance()->GetMyWidget();

	GEngine->GameViewport->AddViewportWidgetContent(MyUI);

}

void AMyPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	if (OnTestBroadCast.IsBound())
	{
		OnTestBroadCast.Broadcast();
	}
}

void AMyPlayerController::RecvProcess(FString& Msg)
{
	FString Temp2 = Msg.TrimStartAndEnd();
	TArray<FString> StringArr;
	Temp2.ParseIntoArray(StringArr, TEXT(","));

	if (StringArr.Num() == 1)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Msg is:--> %s"), *Msg);
	}
	else if (StringArr.Num() == 2)
	{
		for (int i = 0; i < StringArr.Num(); i++)
		{
			if (StringArr[0].Equals(TEXT("chat")))
			{
				DialogString = StringArr[1];
				bTalk = true;
			}
			else if (StringArr[0].Equals(TEXT("action")))
			{
				DialogString = StringArr[1];
				bCanRun = true;
			}
		}
	}
}

APawn* AMyPlayerController::FindNPC_BP()
{
// 	for (TActorIterator<APawn> ActorItr(GetWorld()); ActorItr; ++ActorItr)
// 	{
// 		// Same as with the Object Iterator, access the subclass instance with the * or -> operators.
// 		APawn *actor = *ActorItr;
// 		FString actorlable = actor->GetName();
// 		if (actorlable.Contains(TEXT("kefu")))
// 		{
// 			UActorComponent* AActor = actor->GetComponentByClass(USkeletalMesh::StaticClass());
// 			USkeletalMeshComponent* SMC = Cast<USkeletalMeshComponent>(AActor);
// 			if (SMC)
// 			{
// 				 SMC->PlayAnimation()
// 			}
// 		}
// 	}
	return nullptr;
}

AStaticMeshActor* AMyPlayerController::FindSmallTree()
{
// 	for (TActorIterator<AStaticMeshActor> ActorItr(GetWorld()); ActorItr; ++ActorItr)
// 	{
// 		// Same as with the Object Iterator, access the subclass instance with the * or -> operators.
// 		AStaticMeshActor *actor = *ActorItr;
// 		FString actorlable = actor->GetActorLabel();
// 	}
	return nullptr;
}

void AMyPlayerController::Run()
{
	ACharacter* Cha = GetCharacter();

	if (Cha)
	{
		AServerTestCharacter* STChar = Cast<AServerTestCharacter>(Cha);
		if (STChar)
		{
			FVector STChar_Pos = STChar->GetActorLocation();
		}
	}

}
