#include "Core/TinyRogueGameInstance.h"

#include "Core/TinyRogueProjectSettings.h"
#include "Utility/Log.h"

void UTinyRogueGameInstance::Init()
{
	Super::Init();

	check(false); // Move this to the game mode. We want to be able to select at runtime when we want to use a seed
	
	const UTinyRogueProjectSettings* Settings {UTinyRogueProjectSettings::Get()};
	check(IsValid(Settings));
	Settings->bUsePredeterminedSeed ? Seed.Initialize(Settings->PredeterminedSeed) : Seed.GenerateNewSeed();
}
