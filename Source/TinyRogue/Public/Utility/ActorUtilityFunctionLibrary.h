#pragma once
#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ActorUtilityFunctionLibrary.generated.h"

/**
 * Utility library that provides various actor-related helper functions.
 *
 * This class extends UBlueprintFunctionLibrary, allowing its static utility
 * functions to be used in both C++ and Blueprints. It includes methods for
 * retrieving the current world context and cloning actors across worlds.
 */
UCLASS()
class TINYROGUE_API UActorUtilityFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	/**
	 * Retrieves the current world context associated with the provided object.
	 *
	 * This method is useful for obtaining a UWorld instance, particularly in cases
	 * where the world context is required for further operations in both C++ and
	 * Blueprints.
	 *
	 * @param WorldContextObject The object used to determine the corresponding world context.
	 *                           Typically, this is a UObject that exists within the world hierarchy.
	 * @return A pointer to the current UWorld instance, or nullptr if the world context could not be determined.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure="true", Category="ActorUtilityFunctionLibrary", meta=(WorldContext="WorldContextObject", DefaultToSelf="WorldContextObject", HidePin="WorldContextObject"))
	static UWorld* GetWorldFromObject(UObject* WorldContextObject);

	/**
	 * Clones an actor from the source world to a target world.
	 *
	 * This function spawns a new actor in the specified target world
	 * by duplicating all properties of the provided source actor. If the
	 * target world is not provided or valid, it attempts to deduce the
	 * current world context from the source actor.
	 *
	 * @param SourceActor The actor to be cloned. Must be a valid instance.
	 * @param TargetWorld The world where the cloned actor will be spawned.
	 *                    If invalid, the function attempts to determine the world context automatically.s
	 * @return A pointer to the newly spawned actor copy, or nullptr if cloning was not successful.
	 */
	UFUNCTION(BlueprintCallable, Category="ActorUtilityFunctionLibrary")
	static AActor* CloneActorToWorld(AActor* SourceActor, UWorld* TargetWorld);
	
	UFUNCTION(BlueprintCallable, Category="ActorUtilityFunctionLibrary")
	static AActor* CloneActor(AActor* SourceActor);

	UFUNCTION(BlueprintCallable, Category="ActorUtilityFunctionLibrary", DisplayName="UObject Spawn Actor", meta=(WorldContext="WorldContextObject",
		DefaultToSelf="WorldContextObject", DeterminesOutputType="Class", DynamicOutputParam="ReturnValue"))
	static UObject* UObjectSpawnActor(UObject* WorldContextObject, const TSubclassOf<AActor> Class, const FTransform Transform);
};
