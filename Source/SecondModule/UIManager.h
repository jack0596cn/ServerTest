// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "UIManager.generated.h"

/**
 * 
 */
UCLASS(config = Game)
class SECONDMODULE_API UUIManager : public UObject
{
	GENERATED_BODY()
public:
	static UUIManager* GetInstance()
	{
		if (Singleton == nullptr)
		{
			Singleton = NewObject<UUIManager>();
			Singleton->AddToRoot();
		}
		return Singleton;
	}

	void GrantItems();

	void GrantItemsDeferred();

protected:
	UUIManager();
	virtual ~UUIManager();

private:
	static UUIManager* Singleton;

	TArray<FAssetData> AssetsToLoadList;

	UPROPERTY(Config)
	TArray<FSoftObjectPath> TestAssets;
};
