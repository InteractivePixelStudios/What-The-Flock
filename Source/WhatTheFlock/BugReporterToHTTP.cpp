#include "BugReporterToHTTP.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"

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
	ACharacter* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	if (PlayerCharacter)
	{
		PlayerPosition = PlayerCharacter->GetActorLocation();
		PlayerRotation = PlayerCharacter->GetActorRotation();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Player character not found."));
	}
}

void ABugReporterToHTTP::SendBugReport(const FString& BugDescription, const FString& ToEmail)
{
	FHttpRequestRef Request = FHttpModule::Get().CreateRequest();

	const FString URL = FString::Printf(TEXT("https://api.mailgun.net/v3/%s/messages"), *MailgunDomain);
	const FString AuthHeader = "Basic " + FBase64::Encode("api:" + ApiKey);

	FString BugData = FString::Printf(
		TEXT("from=%s&to=%s&subject=%s&text=%s\nPlayer Position: %s\nPlayer Rotation: %s"),
		*FGenericPlatformHttp::UrlEncode(FString("bugreporter@" + MailgunDomain)),
		*FGenericPlatformHttp::UrlEncode(ToEmail),
		*FGenericPlatformHttp::UrlEncode("Bug Report"),
		*FGenericPlatformHttp::UrlEncode(BugDescription),
		*PlayerPosition.ToString(),
		*PlayerRotation.ToString()
	);


	Request->SetURL(URL);
	Request->SetVerb("POST");
	Request->SetHeader("Authorization", AuthHeader);
	Request->SetHeader("Content-Type", "application/x-www-form-urlencoded");
	Request->SetContentAsString(BugData);

	Request->OnProcessRequestComplete().BindUObject(this, &ABugReporterToHTTP::OnResponseReceived);
	Request->ProcessRequest();
}

void ABugReporterToHTTP::OnResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully)
{
	if (bConnectedSuccessfully && Response.IsValid())
	{
		UE_LOG(LogTemp, Display, TEXT("Bug report sent successfully: %s"), *Response->GetContentAsString());
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to send bug report."));
	}
}
