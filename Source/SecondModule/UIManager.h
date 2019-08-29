// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "UIManager.generated.h"

/**
 * 
 */
UCLASS()
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

protected:
	UUIManager();
	virtual ~UUIManager();

private:
	static UUIManager* Singleton;
};
