#include "BugReporterToHTTP.h"
#include "HttpModule.h"
#include "GameFramework/PlayerController.h"

ABugReporterToHTTP::ABugReporterToHTTP()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ABugReporterToHTTP::BeginPlay()
{
	Super::BeginPlay();
}

void ABugReporterToHTTP::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABugReporterToHTTP::SavePlayerData()
{
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	if (PlayerController)
	{
		PlayerController->GetPlayerViewPoint(PlayerPosition, PlayerRotation);
		UE_LOG(LogTemp, Log, TEXT("Player Data Saved: Position (%s), Rotation (%s)"),
			*PlayerPosition.ToString(), *PlayerRotation.ToString());
	}
}

void ABugReporterToHTTP::SendBugReport(const FString& BugDescription)
{
	if (GoogleFormID.IsEmpty())
	{
		UE_LOG(LogTemp, Warning, TEXT("Google Form ID not set."));
		return;
	}

	// Construct the form submission URL
	FString URL = FString::Printf(TEXT("https://docs.google.com/forms/d/e/%s/formResponse"), *GoogleFormID);

	TSharedRef<IHttpRequest> Request = FHttpModule::Get().CreateRequest();
	Request->SetURL(URL);
	Request->SetVerb("POST");
	Request->SetHeader("Content-Type", "application/x-www-form-urlencoded");

	// Map Google Form entry fields to Unreal Engine data
	FString PlayerData = FString::Printf(TEXT("Position: %s, Rotation: %s"), *PlayerPosition.ToString(), *PlayerRotation.ToString());
	FString Payload = FString::Printf(TEXT("entry.1041133368=%s&entry.1973105176:=%s"),
		*FGenericPlatformHttp::UrlEncode(BugDescription),
		*FGenericPlatformHttp::UrlEncode(PlayerData));

	Request->SetContentAsString(Payload);

	Request->OnProcessRequestComplete().BindUObject(this, &ABugReporterToHTTP::OnResponseReceived);
	Request->ProcessRequest();
}

void ABugReporterToHTTP::OnResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully)
{
	if (bConnectedSuccessfully && Response.IsValid())
	{
		UE_LOG(LogTemp, Log, TEXT("Bug report sent successfully!"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to send bug report."));
	}
}
