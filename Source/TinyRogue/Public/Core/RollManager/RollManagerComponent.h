#pragma once
#include "CoreMinimal.h"
#include "PhysicsSimulationWorldManager.h"
#include "Core/WorldObject.h"
#include "RollManagerComponent.generated.h"

UCLASS(Blueprintable, BlueprintType)
class TINYROGUE_API URollManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, BlueprintPure="false")
	TArray<int32> DetermineDiceValues(const int32 NumDice);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, BlueprintPure="false")
	TArray<FPhysicsSimulationData> SimulateRoll(const int32 NumDice);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void ExecuteRollSimulation(const TArray<int32>& DiceValues);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	UPhysicsWorldSimulationManager* GetPhysicsWorldSimulationManager();
	
protected:
	UPROPERTY()
	TObjectPtr<UPhysicsWorldSimulationManager> PhysicsWorldSimulationManager;
};
