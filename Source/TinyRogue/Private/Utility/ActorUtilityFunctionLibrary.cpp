#include "Utility/ActorUtilityFunctionLibrary.h"
#include "Log.h"

UWorld* UActorUtilityFunctionLibrary::GetWorldFromObject(UObject* WorldContextObject)
{
	if (IsValid(GEngine) == false)
	{
		LOG_ERROR("GEngine is nullptr.")
		return nullptr;
	}

	return GEngine->GetWorldFromContextObjectChecked(WorldContextObject);
}

AActor* UActorUtilityFunctionLibrary::CloneActorToWorld(AActor* SourceActor, UWorld* TargetWorld)
{
	if (IsValid(SourceActor) == false)
	{
		LOG_ERROR("SourceActor is invalid.")
		return nullptr;
	}

	if (IsValid(TargetWorld) == false)
	{
		LOG_ERROR("TargetWorld is invalid.")
		return nullptr;
	}

	FActorSpawnParameters SpawnParams;
	SpawnParams.Template = SourceActor;
	SpawnParams.Owner = SourceActor->GetOwner();
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	SpawnParams.Name = MakeUniqueObjectName(TargetWorld, SourceActor->GetClass(), FName{SourceActor->GetName()}, EUniqueObjectNameOptions::GloballyUnique);
	return TargetWorld->SpawnActor<AActor>(SourceActor->GetClass(), SpawnParams);
}

AActor* UActorUtilityFunctionLibrary::CloneActor(AActor* SourceActor)
{
	if (IsValid(SourceActor) == false)
	{
		LOG_ERROR("SourceActor is invalid.")
		return nullptr;
	}

	UWorld* TargetWorld {GetWorldFromObject(SourceActor)};
	if (IsValid(TargetWorld) == false)
	{
		LOG_ERROR("TargetWorld is invalid.")
		return nullptr;
	}
	
	return CloneActorToWorld(SourceActor, TargetWorld);
}

UObject* UActorUtilityFunctionLibrary::UObjectSpawnActor(UObject* WorldContextObject, const TSubclassOf<AActor> Class, const FTransform Transform)
{
	if (IsValid(Class) == false)
		return nullptr;
	
	UWorld* World {GetWorldFromObject(WorldContextObject)};
	if (IsValid(World) == false)
	{
		LOG_ERROR("World is nullptr.")
		return nullptr;
	}

	return World->SpawnActor<AActor>(Class, Transform);
}
