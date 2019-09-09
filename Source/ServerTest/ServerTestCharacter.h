// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Engine/StaticMeshActor.h"
#include "HMDStaticMeshActor.h"
#include "IPAddress.h"
#include "UnrealNetwork.h"
#include "Sockets.h"
#include "SocketSubsystem.h"
#include "Interfaces/IPv4/IPv4Address.h"
#include "ServerTestCharacter.generated.h"

class FSocket;

UCLASS(config=Game)
class AServerTestCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

public:
	AServerTestCharacter();

	void InitializeInputBindings();

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;
public:
	
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds);
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY()
	AHMDStaticMeshActor* SMActor;
	
	UPROPERTY(/*Replicated*/)
	UStaticMeshComponent* MoveMeshTestCom;

	FSocket* Socket;

public:
	void OnGPress();
	void OnHPress();
	void OnJPress();

	UFUNCTION(NetMulticast, reliable, WithValidation)
	void JumpOnServer();

	UFUNCTION(Server, reliable, WithValidation)
	void OutServerCMD();
	
	UFUNCTION(Server, reliable, WithValidation)
	void LinkOuterServer();

	UFUNCTION(Server, reliable,WithValidation)
	void Mount();

	UFUNCTION(Server, reliable, WithValidation)
	void Demount();

	UFUNCTION(NetMulticast, unreliable, WithValidation)
	void Demount_MulticastRPCFunction();

	UFUNCTION(NetMulticast, unreliable, WithValidation)
	void Mount_MulticastRPCFunction();

	// cmd in console
	UFUNCTION(Exec)
	void MyPrint();

protected:

	/** Resets HMD orientation in VR. */
	void OnResetVR();

	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);

	/** 
	 * Called via input to turn at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

	/** Handler for when a touch input begins. */
	void TouchStarted(ETouchIndex::Type FingerIndex, FVector Location);

	/** Handler for when a touch input stops. */
	void TouchStopped(ETouchIndex::Type FingerIndex, FVector Location);

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// End of APawn interface

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
};

