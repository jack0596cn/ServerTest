// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SMyCompoundWidget.h"
#include "MyUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class SECONDMODULE_API UMyUserWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UMyUserWidget(const FObjectInitializer& ObjectInitializer);
	
	virtual TSharedRef<SWidget> RebuildWidget() override;
private:
	TSharedPtr<SMyCompoundWidget> TestUI;
};
