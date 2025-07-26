#pragma once
#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "SeededRandomFunctionsLibrary.generated.h"

UCLASS()
class TINYROGUE_API USeededRandomFunctionsLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintPure="true", Category="SeededRandomFunctionsLibrary", meta=(WorldContext="WorldContextObject", DefaultToSelf="WorldContextObject"))
	static int32 GetSeededIntegerInRange(const UObject* WorldContextObject, int32 Min, int32 Max);

	UFUNCTION(BlueprintCallable, BlueprintPure="true", Category="SeededRandomFunctionsLibrary", meta=(WorldContext="WorldContextObject", DefaultToSelf="WorldContextObject"))
	static float GetSeededFloatInRange(const UObject* WorldContextObject, float Min, float Max);

	UFUNCTION(BlueprintCallable, BlueprintPure="true", Category="SeededRandomFunctionsLibrary", meta=(WorldContext="WorldContextObject", DefaultToSelf="WorldContextObject"))
	static bool GetSeededBool(const UObject* WorldContextObject);

	UFUNCTION(BlueprintCallable, BlueprintPure="true", Category="SeededRandomFunctionsLibrary", meta=(WorldContext="WorldContextObject", DefaultToSelf="WorldContextObject"))
	static FVector GetSeededUnitVectorInConeInDegrees(const UObject* WorldContextObject, FVector ConeDirection, float ConeHalfAngleInDegrees);

	UFUNCTION(BlueprintCallable, BlueprintPure="true", Category="SeededRandomFunctionsLibrary", meta=(WorldContext="WorldContextObject", DefaultToSelf="WorldContextObject"))
	static FVector GetSeededUnitVector(const UObject* WorldContextObject);

	UFUNCTION(BlueprintCallable, BlueprintPure="true", Category="SeededRandomFunctionsLibrary", meta=(WorldContext="WorldContextObject", DefaultToSelf="WorldContextObject"))
	static FRotator GetSeededRotator(const UObject* WorldContextObject, bool bRoll = true);

private:
	static const FRandomStream& GetSeed(const UObject* WorldContextObject);
};
