// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Networking.h"
#include "TcpReceiverThread.h"
#include "GameFramework/Actor.h"
#include "MyPlayerController.h"
#include "TcpClient.generated.h"

UCLASS()
class SERVERTEST_API ATcpClient : public AActor
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	ATcpClient();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called when the game end
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	UFUNCTION(BlueprintCallable, Category = "Tcp Client")
	bool SocketCreate(FString IPStr = TEXT("172.16.23.61"), int32 port = 32359);

	UFUNCTION(BlueprintCallable, Category = "Tcp Client")
	bool SocketSend(FString message);

	UFUNCTION(BlueprintCallable, Category = "Tcp Client")
	virtual bool SocketReceive();
public:
	/** Update Rate (sec)  */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = ShipParam, Meta = (DisplayName = "UpdateRate"))
	float Tcp_UpdateRate;

public:
	bool ThreadEnd();
	FString StringFromBinaryArray(TArray<uint8> BinaryArray);
	FSocket *Host;
	FIPv4Address ip;
	FRunnableThread* m_RecvThread;
	TcpReceiverThread* m_TcpRecvThread;

	AMyPlayerController* PC;
	
};
