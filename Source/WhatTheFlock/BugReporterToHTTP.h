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
	ABugReporterToHTTP();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	/** Sends a bug report using Google Forms */
	UFUNCTION(BlueprintCallable, Category = "Bug Report")
	void SendBugReport(const FString& BugDescription);

	/** Captures the current player position and rotation */
	UFUNCTION(BlueprintCallable, Category = "Bug Report")
	void SavePlayerData();

private:
	void OnResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully);

	UPROPERTY(EditAnywhere, Category = "Bug Report")
	FString GoogleFormID = "3JFg9DosWzXMvL8o9";

	FVector PlayerPosition;
	FRotator PlayerRotation;
};
