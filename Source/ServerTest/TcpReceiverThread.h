// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ThreadingBase.h"
#include "Networking.h"
#include "ServerTestCharacter.h"
#include "MyPlayerController.h"

/**
 * 
 */
class SERVERTEST_API TcpReceiverThread : public FRunnable
{
public:
	float Tcp_UpdateRate;

public:
	TcpReceiverThread(FSocket* client = NULL, AMyPlayerController* PC = NULL) : m_Client(client), MyController(PC)
	{
		
	}
	~TcpReceiverThread()
	{
		stopping = true;
	}

	virtual bool Init() override
	{
		stopping = false;
		return true;
	}

	virtual uint32 Run() override
	{
		if (!m_Client)
		{
			return 0;
		}

		// Init
		Tcp_UpdateRate = 0.1f;

		TArray<uint8> ReceiveData;
		//uint32 size = 1024u;
		uint32 size = 64u;
		//Receive Data
		while (!stopping)
		{
			while (m_Client->HasPendingData(size))
			{
				//ReceiveData.Init(0, FMath::Min(size, 65507u));
				ReceiveData.Init(0, FMath::Min(size, 64u));
				int32 read = 0;

				m_Client->Recv(ReceiveData.GetData(), ReceiveData.Num(), read, ESocketReceiveFlags::None);
				UE_LOG(LogTemp, Warning, TEXT("Data Bytes Read : %s Byte."), *FString::FromInt(read));

				FString ReceivedUE4String = /*"Data : " + */FString(ANSI_TO_TCHAR(reinterpret_cast<const char*>(ReceiveData.GetData()))); // byte to char
				UE_LOG(LogTemp, Warning, TEXT("%s"), *ReceivedUE4String);

				if (MyController)
				{
					AMyPlayerController* MyPC = Cast<AMyPlayerController>(MyController);
					if (MyPC)
					{
						MyPC->RecvProcess(ReceivedUE4String);
					}
				}
			}

			FPlatformProcess::Sleep(Tcp_UpdateRate);
		}

		return 1;
	}

	virtual void Stop() override
	{
		stopping = true;
	}

protected:
	bool stopping;
	FThreadSafeCounter m_StopTaskCounter;
private:
	FSocket* m_Client;
	APlayerController* MyController;
};
