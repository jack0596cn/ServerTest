// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "SecondModule.h"
#include "Modules/ModuleManager.h"

IMPLEMENT_GAME_MODULE(FSecondeModule, SecondModule);


#define LOCTEXT_NAMESPACE "SecondModule"

void FSecondeModule::StartupModule()
{
	UE_LOG(LogTemp, Warning, TEXT("SecondModule: Log Started"));
}

void FSecondeModule::ShutdownModule()
{
	UE_LOG(LogTemp, Warning, TEXT("SecondModule: Log Ended"));
}

#undef LOCTEXT_NAMESPACE