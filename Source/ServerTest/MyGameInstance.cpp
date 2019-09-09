// Fill out your copyright notice in the Description page of Project Settings.

#include "MyGameInstance.h"

UMyGameInstance::UMyGameInstance(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

}

void UMyGameInstance::Init()
{
	UE_LOG(LogTemp, Log, TEXT("Enter GameInstance Init!"));	
}


