// Fill out your copyright notice in the Description page of Project Settings.

#include "MyGameInstance.h"

//FSocket* UMyGameInstance::Socket = nullptr;

UMyGameInstance::UMyGameInstance(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
// 	Socket = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateSocket(NAME_Stream, TEXT("default"), false);
// 
// 	FString address = TEXT("127.0.0.1");
// 	int32 port = 8888;
// 	FIPv4Address ip;
// 	FIPv4Address::Parse(address, ip);
// 
// 	TSharedRef<FInternetAddr> Addr = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr();
// 	Addr->SetIp(ip.Value);
// 	Addr->SetPort(port);
// 
// 	if (!Socket->Connect(*Addr))
// 	{
// 		UE_LOG(LogTemp, Log, TEXT("Connect 127.0.0.1 8888 failed!"));
// 	}
}


