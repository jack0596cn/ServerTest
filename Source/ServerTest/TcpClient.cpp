// Fill out your copyright notice in the Description page of Project Settings.

#include "TcpClient.h"
#include "UnrealNetwork.h"

// Sets default values
ATcpClient::ATcpClient()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	Tcp_UpdateRate = 0.1f;

	//this->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	
}

// Called when the game starts or when spawned
void ATcpClient::BeginPlay()
{
	Super::BeginPlay();

	APlayerController* playercontro = GEngine->GetFirstLocalPlayerController(GetWorld());
	PC = Cast<AMyPlayerController>(playercontro);
	
	SocketCreate();
	SocketReceive();
	
}

// Replicate
void ATcpClient::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ATcpClient, Tcp_UpdateRate);
}

// Called every frame
void ATcpClient::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (m_TcpRecvThread)
	{
		m_TcpRecvThread->Tcp_UpdateRate = Tcp_UpdateRate;
	}
}

void ATcpClient::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	ThreadEnd();

	if (Host)
	{
		Host->Close();
		ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->DestroySocket(Host);
	}

	Super::EndPlay(EndPlayReason);
}

bool ATcpClient::SocketCreate(FString IPStr, int32 port)
{
	FIPv4Address::Parse(IPStr, ip);

	TSharedPtr<FInternetAddr> addr = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr();
	addr->SetIp(ip.Value);
	addr->SetPort(port);

	//Create Client Socket
	Host = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateSocket(NAME_Stream, TEXT("default"), false);

	//Connect Success
	if (Host->Connect(*addr))
	{
		UE_LOG(LogTemp, Warning, TEXT("TCP Connect Succeed!"));
		return true;
	}
	//Connect Failed
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("TCP Connect Failed!"));
		return false;
	}
}

bool ATcpClient::SocketSend(FString message)
{
	TCHAR *seriallizedChar = message.GetCharArray().GetData();
	int32 size = FCString::Strlen(seriallizedChar) + 1;
	int32 sent = 0;

	if (Host->Send((uint8*)TCHAR_TO_UTF8(seriallizedChar), size, sent))
	{
		UE_LOG(LogTemp, Warning, TEXT("___Send Succeed!"));
		return true;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("___Send Failed!"));
		return false;
	}
}

bool ATcpClient::SocketReceive()
{
	if (Host->GetConnectionState() == ESocketConnectionState::SCS_Connected)
	{
		FThreadSafeCounter  WorkerCounter;

		FString ThreadName(FString::Printf(TEXT("RecvThread%i"), WorkerCounter.Increment()));

		m_TcpRecvThread = new TcpReceiverThread(Host, PC);
		m_RecvThread = FRunnableThread::Create(m_TcpRecvThread, *ThreadName);

		return true;
	}

	return false;
}

bool ATcpClient::ThreadEnd()
{
	if (m_RecvThread != nullptr)
	{
		m_RecvThread->Kill(true);
		delete m_RecvThread;
	}
	return true;
}

FString ATcpClient::StringFromBinaryArray(TArray<uint8> BinaryArray)
{
	return FString(ANSI_TO_TCHAR(reinterpret_cast<const char*>(BinaryArray.GetData())));
}
