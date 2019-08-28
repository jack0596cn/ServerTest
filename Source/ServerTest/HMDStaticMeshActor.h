// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/StaticMeshActor.h"
#include "Engine/StaticMesh.h"
#include "Components/TextRenderComponent.h"
#include "MyObject.h"
#include "HMDStaticMeshActor.generated.h"

/**
 * 
 */
UCLASS()
class SERVERTEST_API AHMDStaticMeshActor : public AStaticMeshActor
{
	GENERATED_BODY()

public:
	AHMDStaticMeshActor();
	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds);
	void OneSecTick();
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
public:
	UStaticMeshComponent* GetSMC()
	{
		return SMCom;
	}

private:
	UPROPERTY()
	AStaticMeshActor* SMActor;
	
	UPROPERTY()
	UStaticMeshComponent* SMCom;

	UPROPERTY()
	UTextRenderComponent* TRCCom;

	FTimerHandle OneSecTimerHandle;
private:
	UPROPERTY(Replicated)
	uint32 CountDownTimer;

	UPROPERTY(EditAnywhere)
	FString Test;

	UPROPERTY(EditAnywhere)
	AMyObject *pObject;
	
	UPROPERTY(EditAnywhere)
	FTest info;
};
