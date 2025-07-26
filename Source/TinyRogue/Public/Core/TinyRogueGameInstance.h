#pragma once
#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "TinyRogueGameInstance.generated.h"

UCLASS()
class TINYROGUE_API UTinyRogueGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	virtual void Init() override;
	
	UFUNCTION(BlueprintCallable, BlueprintPure="true", Category="TinyRogueGameInstance|Seed")
	const FRandomStream& GetSeed() const { return Seed; }
	
	UFUNCTION(BlueprintCallable, Category="TinyRogueGameInstance|Seed")
	void SetSeed(const FRandomStream NewSeed) { Seed = NewSeed; };

private:
	FRandomStream Seed;
};
