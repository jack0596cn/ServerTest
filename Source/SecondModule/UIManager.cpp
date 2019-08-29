// Fill out your copyright notice in the Description page of Project Settings.

#include "UIManager.h"

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
}
