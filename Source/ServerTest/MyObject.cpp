// Fill out your copyright notice in the Description page of Project Settings.

#include "MyObject.h"

AMyObject::AMyObject()
{
	PrimaryActorTick.bCanEverTick = true;
	DefaultString = TEXT("DefualtString....");
}

void AMyObject:: Tick(float DeltaSeconds)
{
	OnShowDebugInfo();
	CallBPLogic();
	Super::Tick(DeltaSeconds);
}

void AMyObject::OnShowDebugInfo_Implementation()
{
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("Show DebugInfo"));
}


