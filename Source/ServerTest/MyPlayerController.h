// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Engine/StaticMeshActor.h"
#include "MyPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class SERVERTEST_API AMyPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	AMyPlayerController(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual void PlayerTick(float DeltaTime);

	virtual void BeginPlay() override;

	void RecvProcess(FString& Msg);

	UFUNCTION(BlueprintCallable, Category = "ServerTest")
	bool TCP_CanRun()
	{
		return bCanRun;
	}

	UFUNCTION(BlueprintCallable, Category = "ServerTest")
	void Set_TCP_CanRun(bool bRun)
	{
		bCanRun = bRun;
	}

	UFUNCTION(BlueprintCallable, Category = "ServerTest")
	bool TCP_CanTalk()
	{
		return bTalk;
	}

	UFUNCTION(BlueprintCallable, Category = "ServerTest")
	void Set_TCP_CanTalk(bool _bTalk)
	{
		bTalk = _bTalk;
	}

	UFUNCTION(BlueprintCallable, Category = "ServerTest")
	FString TCP_GetServerString()
	{
		return DialogString;
	}

	void Talk();
	void Run();
	AStaticMeshActor* FindSmallTree();
	APawn* FindNPC_BP();


public:

	bool bCanRun;
	bool bTalk;
	FString DialogString;
};
