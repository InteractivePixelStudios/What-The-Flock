#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Http.h"
#include "BugReporterToHTTP.generated.h"

UCLASS()
class WHATTHEFLOCK_API ABugReporterToHTTP : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABugReporterToHTTP();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	// Method to send bug report data via HTTP
	UFUNCTION(BlueprintCallable, Category = "Bug Report")
	void SendBugReport(const FString& BugDescription, const FString& ToEmail);

	// Save player position and rotation
	UFUNCTION(BlueprintCallable, Category = "Bug Report")
	void SavePlayerData();

private:
	void OnResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully);

	UPROPERTY(EditAnywhere, Category = "Mailgun")
	FString MailgunDomain = "sandbox7c8b5dcbcf1c4661ac33053ecfd3d32e.mailgun.org";

	UPROPERTY(EditAnywhere, Category = "Mailgun")
	FString ApiKey = "";

	// Player position and rotation
	FVector PlayerPosition;
	FRotator PlayerRotation;
};
