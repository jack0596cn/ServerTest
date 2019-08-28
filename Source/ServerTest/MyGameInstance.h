// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Sockets.h"
#include "SocketSubsystem.h"
#include "Interfaces/IPv4/IPv4Address.h"
#include "IPAddress.h"
#include "MyGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class SERVERTEST_API UMyGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UMyGameInstance(const FObjectInitializer& ObjectInitializer);

	FSocket* GetSocket()
	{
		return Socket;
	}

private:
	FSocket* Socket;
	
};
