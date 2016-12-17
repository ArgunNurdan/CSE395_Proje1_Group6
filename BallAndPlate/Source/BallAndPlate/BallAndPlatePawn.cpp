// Fill out your copyright notice in the Description page of Project Settings.

#include "BallAndPlate.h"
#include "BallAndPlatePawn.h"
#include <thread>
#include <string>

float a=0.0f;

float differenceX_Z;
float differenceY_Z;
FRotator rotation;
// Sets default values
ABallAndPlatePawn::ABallAndPlatePawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	AutoPossessPlayer = EAutoReceiveInput::Player0;

	// Create a dummy root component we can attach things to.
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	OurVisibleComponent1 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("OurVisibleComponent1"));
	OurVisibleComponent2 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("OurVisibleComponent2"));

	OurVisibleComponent1->SetupAttachment(RootComponent);// AttachTo(RootComponent);
	OurVisibleComponent2->SetupAttachment(RootComponent); // AttachTo(RootComponent);
	
}

// Called when the game starts or when spawned
void ABallAndPlatePawn::BeginPlay()
{
	Super::BeginPlay();
	differenceX_Z = kolXReference->GetActorLocation().Z - kolX->GetActorLocation().Z;
	differenceY_Z = kolYReference->GetActorLocation().Z - kolY->GetActorLocation().Z;
	rotation = FRotator(RootComponent->GetRelativeTransform().GetRotation());

	setUpLights();

}

// Called every frame
void ABallAndPlatePawn::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

	if (!CurrentVelocity.IsZero())
	{
		FVector refXLocation = kolXReference->GetActorLocation();;
		FVector armXLocation = kolX->GetActorLocation();
		armXLocation.Z = refXLocation.Z - differenceX_Z;
		kolX->SetActorLocation(armXLocation);
		
		FVector refYLocation = kolYReference->GetActorLocation();;
		FVector armYLocation = kolY->GetActorLocation();
		armYLocation.Z = refYLocation.Z - differenceY_Z;
		kolY->SetActorLocation(armYLocation);
		
		RootComponent->SetRelativeRotation(rotation);			
	}
}

// Called to bind functionality to input
void ABallAndPlatePawn::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{

	Super::SetupPlayerInputComponent(InputComponent);

	// Respond every frame to the values of our two movement axes, "MoveX" and "MoveY".
	InputComponent->BindAxis("MoveX", this, &ABallAndPlatePawn::Move_XAxis);
	InputComponent->BindAxis("MoveY", this, &ABallAndPlatePawn::Move_YAxis);

}

void ABallAndPlatePawn::Move_XAxis(float AxisValue)
{
	// Move at 100 units per second forward or backward
	CurrentVelocity.X = FMath::Clamp(AxisValue, -1.0f, 10.0f) * 100.0f;
	rot.X = FMath::Clamp(AxisValue, -1.0f, 10.0f) * 5.0f;
	rotation.Pitch += (a = AxisValue + 0.01f);
}

void ABallAndPlatePawn::Move_YAxis(float AxisValue)
{
	// Move at 100 units per second right or left
	CurrentVelocity.Y = FMath::Clamp(AxisValue, -1.0f, 1.0f) * 100.0f;
	rot.Y = FMath::Clamp(AxisValue, -1.0f, 1.0f)* 5.0f;
	rotation.Roll += (a = AxisValue + 0.01f);
}

void ABallAndPlatePawn::setUpLights()
{
	int i = 0, j=0;

	lights.Add(UprightLights1);
	lights.Add(UprightLights2);
	lights.Add(UprightLights3);
	lights.Add(UprightLights4);
	lights.Add(UprightLights5);
	lights.Add(UprightLights6);
	lights.Add(UprightLights7);
	lights.Add(UprightLights8);
	lights.Add(UprightLights9);
	lights.Add(UprightLights10);
	lights.Add(UprightLights11);
	lights.Add(UprightLights12);
	lights.Add(UprightLights13);


	for (i = 0; i < lights.Num(); ++i) {
		for (j = 0; j < lights[i].Num(); ++j) {
			if (lights[i][j] != nullptr) {
				lights[i][j]->SetEnabled(false);
			}
			
		}
	}
}












void ABallAndPlatePawn::Launcch()
{
	//IP = 127.0.0.1, Port = 8890 for my Python test case
	if (!StartTCPReceiver("RamaSocketListener", "127.0.0.1", 8890))
	{
		//UE_LOG  "TCP Socket Listener Created!"
		return;
	}

	//UE_LOG  "TCP Socket Listener Created! Yay!"
}

//Rama's Start TCP Receiver
bool ABallAndPlatePawn::StartTCPReceiver(
	const FString& YourChosenSocketName,
	const FString& TheIP,
	const int32 ThePort
	) {
	//Rama's CreateTCPConnectionListener
	ListenerSocket = CreateTCPConnectionListener(YourChosenSocketName, TheIP, ThePort);

	//Not created?
	if (!ListenerSocket)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("StartTCPReceiver>> Listen socket could not be created! ~> %s %d"), *TheIP, ThePort));
		return false;
	}

	//Start the Listener! //thread this eventually
	GetWorldTimerManager().SetTimer(timeHandler, this,
		&ABallAndPlatePawn::TCPConnectionListener, 0.01, true);

	return true;
}
//Format IP String as Number Parts
bool ABallAndPlatePawn::FormatIP4ToNumber(const FString& TheIP, uint8(&Out)[4])
{
	//IP Formatting
	TheIP.Replace(TEXT(" "), TEXT(""));

	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	//						   IP 4 Parts

	//String Parts
	TArray<FString> Parts;
	//const wchar_t ar;

	TheIP.ParseIntoArray(Parts, TEXT("."), true);
	if (Parts.Num() != 4)
		return false;

	//String to Number Parts
	for (int32 i = 0; i < 4; ++i)
	{
		Out[i] = FCString::Atoi(*Parts[i]);
	}

	return true;
}
//Rama's Create TCP Connection Listener
FSocket* ABallAndPlatePawn::CreateTCPConnectionListener(const FString& YourChosenSocketName, const FString& TheIP, const int32 ThePort, const int32 ReceiveBufferSize)
{
	uint8 IP4Nums[4];
	if (!FormatIP4ToNumber(TheIP, IP4Nums))
	{
		//VShow("Invalid IP! Expecting 4 parts separated by .");
		return false;
	}

	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

	//Create Socket
	FIPv4Endpoint Endpoint(FIPv4Address(IP4Nums[0], IP4Nums[1], IP4Nums[2], IP4Nums[3]), ThePort);
	FSocket* ListenSocket = FTcpSocketBuilder(*YourChosenSocketName)
		.AsReusable()
		.BoundToEndpoint(Endpoint)
		.Listening(8);

	//Set Buffer Size
	int32 NewSize = 0;
	ListenSocket->SetReceiveBufferSize(ReceiveBufferSize, NewSize);

	//Done!
	return ListenSocket;
}
//Rama's TCP Connection Listener
void ABallAndPlatePawn::TCPConnectionListener()
{
	//~~~~~~~~~~~~~
	if (!ListenerSocket) return;
	//~~~~~~~~~~~~~

	//Remote address
	TSharedRef<FInternetAddr> RemoteAddress = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr();
	bool Pending;

	// handle incoming connections
	if (ListenerSocket->HasPendingConnection(Pending) && Pending)
	{
		//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
		//Already have a Connection? destroy previous
		if (ConnectionSocket)
		{
			ConnectionSocket->Close();
			ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->DestroySocket(ConnectionSocket);
		}
		//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

		//New Connection receive!
		ConnectionSocket = ListenerSocket->Accept(*RemoteAddress, TEXT("RamaTCP Received Socket Connection"));

		if (ConnectionSocket != NULL)
		{
			//Global cache of current Remote Address
			RemoteAddressForConnection = FIPv4Endpoint(RemoteAddress);

			//UE_LOG "Accepted Connection! WOOOHOOOO!!!";

			//can thread this too
			//FTimerDelegate MyDel;
			
//			MyDel.BindRaw(this, &ABallAndPlatePawn::TCPSocketListener);
				GetWorldTimerManager().SetTimer(timeHandler, this,
				&ABallAndPlatePawn::TCPConnectionListener, 0.01f, true);
				//SetTimer(MyDel, 0.01, true);
		}
	}
}

//Rama's TCP Socket Listener
void ABallAndPlatePawn::TCPSocketListener()
{
	//~~~~~~~~~~~~~
	if (!ConnectionSocket) return;
	//~~~~~~~~~~~~~


	//Binary Array!
	TArray<uint8> ReceivedData;

	uint32 Size;
	while (ConnectionSocket->HasPendingData(Size))
	{

		ReceivedData.Init(0,FMath::Min(Size, 65507u));

		int32 Read = 0;
		ConnectionSocket->Recv(ReceivedData.GetData(), ReceivedData.Num(), Read);

		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Data Read! %d"), ReceivedData.Num()));
	}
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

	if (ReceivedData.Num() <= 0)
	{
		//No Data Received
		return;
	}

	//VShow("Total Data read!", ReceivedData.Num());
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Data Bytes Read ~> %d"), ReceivedData.Num()));


	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	//						Rama's String From Binary Array
	const FString ReceivedUE4String = StringFromBinaryArray(ReceivedData);
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


	//VShow("As String!!!!! ~>", ReceivedUE4String);
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("As String Data ~> %s"), *ReceivedUE4String));
}

FString ABallAndPlatePawn::StringFromBinaryArray(const TArray<uint8>& BinaryArray)
{
	//Create a string from a byte array!
	const std::string cstr(reinterpret_cast<const char*>(BinaryArray.GetData()), BinaryArray.Num());

	//FString can take in the c_str() of a std::string
	return FString(cstr.c_str());
}
