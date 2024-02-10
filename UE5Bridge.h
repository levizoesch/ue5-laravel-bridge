#pragma once

#include "CoreMinimal.h"
#include "UE5Bridge.generated.h"

UCLASS()
class MYUE5PROJECT_API UUE5Bridge : public UObject
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable, Category="UE5Bridge")
	static FString GetCurrentTime();

	UFUNCTION(BlueprintCallable, Category="UE5Bridge")
	static FString GenerateDynamicTokenFromTime(FString IncomingTime);

	UFUNCTION(BlueprintCallable, Category="UE5Bridge")
	static FString HashToken(const FString& DynamicToken);

	UFUNCTION(BlueprintCallable, Category="UE5Bridge")
	static TMap<FString, FString> GetHashedToken(const TMap<FString, FString>& Params);
};