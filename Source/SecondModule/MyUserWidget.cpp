// Fill out your copyright notice in the Description page of Project Settings.

#include "MyUserWidget.h"

UMyUserWidget::UMyUserWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

}

TSharedRef<SWidget> UMyUserWidget::RebuildWidget()
{
	TestUI = SNew(SMyCompoundWidget);

	return TestUI.ToSharedRef();
}
