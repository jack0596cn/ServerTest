// Fill out your copyright notice in the Description page of Project Settings.

#include "MyPlayerController.h"
#include "UIManager.h"

AMyPlayerController::AMyPlayerController(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void AMyPlayerController::BeginPlay()
{
	Super::BeginPlay();

	TSharedRef<SWidget> MyUI = UUIManager::GetInstance()->GetMyWidget();

	GEngine->GameViewport->AddViewportWidgetContent(MyUI);
}

void AMyPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);
}


