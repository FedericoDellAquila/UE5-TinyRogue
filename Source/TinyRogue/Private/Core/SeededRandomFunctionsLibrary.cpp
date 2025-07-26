#include "Core/SeededRandomFunctionsLibrary.h"
#include "Core/TinyRogueGameInstance.h"
#include "Kismet/KismetMathLibrary.h"

int32 USeededRandomFunctionsLibrary::GetSeededIntegerInRange(const UObject* WorldContextObject, const int32 Min, const int32 Max)
{
	return UKismetMathLibrary::RandomIntegerInRangeFromStream(GetSeed(WorldContextObject), Min, Max);
}

float USeededRandomFunctionsLibrary::GetSeededFloatInRange(const UObject* WorldContextObject, const float Min, const float Max)
{
	return UKismetMathLibrary::RandomFloatInRangeFromStream(GetSeed(WorldContextObject), Min, Max);
}

bool USeededRandomFunctionsLibrary::GetSeededBool(const UObject* WorldContextObject)
{
	return UKismetMathLibrary::RandomBoolFromStream(GetSeed(WorldContextObject));
}

FVector USeededRandomFunctionsLibrary::GetSeededUnitVectorInConeInDegrees(const UObject* WorldContextObject, const FVector ConeDirection, const float ConeHalfAngleInDegrees)
{
	return UKismetMathLibrary::RandomUnitVectorInConeInDegreesFromStream(GetSeed(WorldContextObject), ConeDirection, ConeHalfAngleInDegrees);
}

FVector USeededRandomFunctionsLibrary::GetSeededUnitVector(const UObject* WorldContextObject)
{
	return UKismetMathLibrary::RandomUnitVectorFromStream(GetSeed(WorldContextObject));
}

FRotator USeededRandomFunctionsLibrary::GetSeededRotator(const UObject* WorldContextObject, const bool bRoll/*= true*/)
{
	return UKismetMathLibrary::RandomRotatorFromStream(GetSeed(WorldContextObject), bRoll);
}

const FRandomStream& USeededRandomFunctionsLibrary::GetSeed(const UObject* WorldContextObject)
{
	check(IsValid(GEngine));
	const UWorld* World {GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::Assert)};
	const UTinyRogueGameInstance* GameMode {Cast<UTinyRogueGameInstance>(World->GetGameInstance())};
	check(IsValid(GameMode));
	return GameMode->GetSeed();
}
