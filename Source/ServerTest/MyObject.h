// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/StaticMeshActor.h"
#include "MyObject.generated.h"


USTRUCT()
struct SERVERTEST_API FTest
{
public:
	GENERATED_USTRUCT_BODY()

	FTest()
	{

	}

	virtual ~FTest()
	{

	}

	UPROPERTY(EditAnywhere, Category = Instances)
	FMatrix Transform;

	UPROPERTY(EditAnywhere)
	FString Name;
};

/**
 * 
 */
UCLASS()
class SERVERTEST_API AMyObject : public AActor
{
	GENERATED_BODY()
	
public:

	UPROPERTY(EditAnywhere)
	FString Test;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AStaticMeshActor> SM;

	UPROPERTY(EditAnywhere)
	FTest TT;
};
