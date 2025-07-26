#pragma once
#include "CoreMinimal.h"
#include "GameFramework/CheatManager.h"
#include "TinyRogueCheatManager.generated.h"

UCLASS()
class TINYROGUE_API UTinyRogueCheatManager : public UCheatManager
{
	GENERATED_BODY()

public:
	/**
	 * Simulates a die roll based on a string of space-separated values. Each numeric value in the provided
	 * string is parsed and processed as an individual dice roll result. Non-numeric values are ignored.
	 *
	 * @param Values A space-separated string containing numerical values to simulate dice rolls.
	 *               Non-numeric entries are disregarded.
	 */
	void RollDice(const TArray<FString>& Values) const;

	/**
	 * Returns the current Game seed.
	 */
	void PrintSeed() const;
};
