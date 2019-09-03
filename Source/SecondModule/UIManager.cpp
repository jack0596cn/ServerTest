// Fill out your copyright notice in the Description page of Project Settings.

#include "UIManager.h"
#include "Classes/Engine/AssetManager.h"
#include "Engine/StaticMeshActor.h"
#include "Engine/StaticMesh.h"

UUIManager* UUIManager::Singleton = nullptr;

UUIManager::UUIManager()
{
}

UUIManager::~UUIManager()
{
	if (Singleton)
	{
		Singleton->RemoveFromRoot();
	}

	if (MyUserWidget)
	{
		MyUserWidget->RemoveFromRoot();
	}
}

void UUIManager::GrantItems()
{
	FStreamableManager& Streamable = UAssetManager::GetStreamableManager();
	Streamable.RequestAsyncLoad(TestAssets, FStreamableDelegate::CreateUObject(this, &UUIManager::GrantItemsDeferred));
}

void UUIManager::GrantItemsDeferred()
{
	for (FSoftObjectPath SoftObj : TestAssets)
	{
		TAssetPtr<AStaticMeshActor> SMAsset(SoftObj);

		AStaticMeshActor* SMObj = SMAsset.Get();
		if (SMObj)
		{
			GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Red, SMObj->GetName());
		}
	}
}