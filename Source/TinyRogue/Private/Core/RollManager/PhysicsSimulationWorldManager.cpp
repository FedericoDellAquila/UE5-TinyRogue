#include "Core/RollManager/PhysicsSimulationWorldManager.h"
#include "Physics/Experimental/PhysScene_Chaos.h"
#include "Utility/Log.h"
#include "Utility/UtilityFunctionsLibrary.h"

FPhysicsSimulationParameters::FPhysicsSimulationParameters()
	: Gravity(FVector(0.0f, 0.0f, -980.0f))
	  , MinPhysicsDeltaTime(0)
	  , MaxPhysicsDeltaTime(1.0f / 30.0f)
	  , MaxSubsteps(6)
	  , bSubstepping(false)
{
	DeltaSeconds = UUtilityFunctionsLibrary::GetDefaultPhysicsStepDeltaTime();
	MaxSubstepDeltaTime = DeltaSeconds;
}

UPhysicsWorldSimulationManager::UPhysicsWorldSimulationManager()
{}

bool UPhysicsWorldSimulationManager::CreateSimulationWorld()
{
	if (IsValid(SimulationWorld))
		return true;

	// Create a brand new world with a unique name and transient package
	const FName UniqueWorldName {MakeUniqueObjectName(this, UWorld::StaticClass(), TEXT("SimulationWorld"))};

	const UWorld::InitializationValues InitValues
	{
		UWorld::InitializationValues()
		.AllowAudioPlayback(false)
		.RequiresHitProxies(false)
		.CreateNavigation(false)
		.CreateAISystem(false)
		.ShouldSimulatePhysics(false)
		.EnableTraceCollision(true)
		.SetTransactional(false)
		.CreateFXSystem(false)
		.SetDefaultGameMode(nullptr)
	};

	SimulationWorld = UWorld::CreateWorld(EWorldType::Game, false, UniqueWorldName, GetTransientPackage(), true, ERHIFeatureLevel::Num, &InitValues);
	if (IsValid(SimulationWorld) == false)
	{
		LOG_ERROR("Failed to create simulation world.")
		return false;
	}
	SimulationWorld->SetShouldTick(false);

	// Set world context
	FWorldContext& SimulationWorldContext {GEngine->CreateNewWorldContext(EWorldType::Game)};
	SimulationWorldContext.SetCurrentWorld(SimulationWorld);

	return IsValid(SimulationWorld);
}

// TODO: improve CopyStaticActors and CopyPhysicsActors. There's some repeating code that can be avoided

TArray<AActor*> UPhysicsWorldSimulationManager::CopyStaticActors(TArray<AActor*> InStaticActors)
{
	if (InStaticActors.Num() == 0)
		return {};
	
	TArray<AActor*> Clones;
	Clones.Reserve(InStaticActors.Num());
	for (AActor* Actor : InStaticActors)
	{
		if (IsValid(Actor) == false)
		{
			LOG_ERROR("Actor is invalid.")
			continue;
		}
		
		if (Actor->IsRootComponentStatic() == false)
		{
			LOG_WARNING("{0} was not copied since mobility is not `{1}`.", GetNameSafe(Actor), UENUM_TO_STRING(EComponentMobility::Static))
			continue;
		}
		DuplicateActor(Actor, Clones, GET_FUNCTION_NAME_CHECKED(UPhysicsWorldSimulationManager, OnStaticActorDestroyed));
	}

	StaticActors.Append(Clones);

	return Clones;
}

TArray<AActor*> UPhysicsWorldSimulationManager::CopyPhysicsActors(TArray<AActor*> InPhysicsActors)
{
	if (InPhysicsActors.Num() == 0)
		return {};

	TArray<AActor*> Clones;
	Clones.Reserve(InPhysicsActors.Num());
	for (AActor* Actor : InPhysicsActors)
	{
		if (IsValid(Actor) == false)
		{
			LOG_ERROR("Actor is invalid.")
			continue;
		}
		
		if (Actor->IsRootComponentMovable() == false)
		{
			LOG_WARNING("{0} was not copied since mobility is not `{1}`.", GetNameSafe(Actor), UENUM_TO_STRING(EComponentMobility::Movable))
			continue;
		}
		DuplicateActor(Actor, Clones, GET_FUNCTION_NAME_CHECKED(UPhysicsWorldSimulationManager, OnPhysicsActorDestroyed));
	}

	PhysicsActors.Append(Clones);

	return Clones;
}

void UPhysicsWorldSimulationManager::SpawnPhysicsActors(const TSubclassOf<AActor> ActorClass, TArray<FTransform> Transforms, TArray<AActor*>& SpawnedPhysicsActors)
{
	if (IsValid(SimulationWorld) == false)
	{
		LOG_ERROR("SimulationWorld is nullptr.")
		return;
	}

	if (IsValid(ActorClass) == false)
	{
		LOG_ERROR("ActorClass is nullptr.")
		return;
	}

	for (const FTransform& Transform : Transforms)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		AActor* NewActor {SimulationWorld->SpawnActor<AActor>(ActorClass, Transform, SpawnParams)};
		NewActor->OnDestroyed.AddUniqueDynamic(this, &ThisClass::OnPhysicsActorDestroyed);
		SpawnedPhysicsActors.Emplace(NewActor);
	}

	PhysicsActors.Append(SpawnedPhysicsActors);
}

TArray<FPhysicsSimulationData> UPhysicsWorldSimulationManager::PerformPhysicsSimulation(FPhysicsSimulationParameters PhysicsSimulationParameters, int32 MaxSteps/*= 500*/, bool bAutoDestroySimulationWorld/*= true*/)
{
	if (IsValid(SimulationWorld) == false || SimulationWorld->GetPhysicsScene() == nullptr)
	{
		LOG_ERROR("SimulationWorld is nullptr or PhysicsScene is nullptr.")
		return {};
	}

	if (MaxSteps <= 0)
	{
		LOG_ERROR("MaxSteps <= 0.")
		return {};
	}

	TArray<FPhysicsSimulationData> PhysicsSimulations;
	PhysicsSimulations.Reserve(PhysicsActors.Num());
	for (int32 i = 0; i < PhysicsActors.Num(); i++)
	{
		AActor*& Actor {PhysicsActors[i]};

		FPhysicsSimulationData PhysicsSimulation {};
		PhysicsSimulation.Actor = Actor;
		PhysicsSimulation.bIsAsleep = false;
		PhysicsSimulation.Id = i;
		PhysicsSimulation.DeltaTime = PhysicsSimulationParameters.DeltaSeconds;
		Actor->GetComponents(UMeshComponent::StaticClass(), PhysicsSimulation.MeshComponents, true);
		PhysicsSimulations.Emplace(PhysicsSimulation);
	}

	SetUpPhysicsSimulationFrame(PhysicsSimulationParameters);
	
	FPhysScene* PhysicsScene {SimulationWorld->GetPhysicsScene()};
	bool bAllPhysicsActorsAtRest {false};
	int32 StepsCount {0};
	int32 Sleepers {0};

	SimulationWorld->StartPhysicsSim();
	while (bAllPhysicsActorsAtRest == false && StepsCount < MaxSteps)
	{
		PhysicsScene->StartFrame();
		PhysicsScene->WaitPhysScenes();

		for (FPhysicsSimulationData& TransformPhysicsSteps : PhysicsSimulations)
		{
			if (TransformPhysicsSteps.bIsAsleep)
				continue;

			int32 Count {TransformPhysicsSteps.MeshComponents.Num()};
			for (UPrimitiveComponent* MeshComp : TransformPhysicsSteps.MeshComponents)
			{
				if (MeshComp->IsAnyRigidBodyAwake() == false)
					Count--;
			}
			if (Count == 0)
			{
				TransformPhysicsSteps.bIsAsleep = true;

				Sleepers++;
				if (Sleepers == PhysicsSimulations.Num())
				{
					bAllPhysicsActorsAtRest = true;
					break;
				}
			}
		}

		// Record transforms for this step
		for (FPhysicsSimulationData& PhysicsSimulation : PhysicsSimulations)
		{
			if (IsValid(PhysicsSimulation.Actor) && PhysicsSimulation.bIsAsleep == false)
				PhysicsSimulation.Steps.Emplace(PhysicsSimulation.Actor->GetActorTransform());
		}

		PhysicsScene->EndFrame();
		StepsCount++;
	}

	SimulationWorld->FinishPhysicsSim();

	if (bAutoDestroySimulationWorld)
		DestroySimulationWorld();

	return PhysicsSimulations;
}

bool UPhysicsWorldSimulationManager::DestroySimulationWorld()
{
	if (IsValid(SimulationWorld))
	{
		GEngine->DestroyWorldContext(SimulationWorld);
		SimulationWorld->RemoveFromRoot();
		SimulationWorld->DestroyWorld(false);
		SimulationWorld = nullptr;

		// Clear static and physics actors
		StaticActors.Empty();
		PhysicsActors.Empty();
	}
	return IsValid(SimulationWorld) == false;
}

void UPhysicsWorldSimulationManager::DuplicateActor(AActor* SourceActor, TArray<AActor*>& DestinationList, const FName& FunctionName/*= NAME_None*/)
{
	if (IsValid(SimulationWorld) == false)
	{
		LOG_ERROR("SimulationWorld is nullptr.")
		return;
	}

	if (IsValid(SourceActor) == false)
	{
		LOG_ERROR("SourceActor is nullptr.")
		return;
	}

	UClass* SourceActorClass {SourceActor->GetClass()};

	FActorSpawnParameters SpawnParams;
	SpawnParams.Template = SourceActor;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	AActor* NewActor {SimulationWorld->SpawnActor<AActor>(SourceActorClass, SpawnParams)};
	if (IsValid(NewActor) == false)
	{
		LOG_ERROR("Failed to duplicate {0}", SourceActor->GetName());
		return;
	}

	if (FunctionName.IsNone() == false)
	{
		FScriptDelegate ScriptDelegate;
		ScriptDelegate.BindUFunction(this, FunctionName);
		NewActor->OnDestroyed.AddUnique(ScriptDelegate);
	}

	DestinationList.Emplace(NewActor);
}

void UPhysicsWorldSimulationManager::SetUpPhysicsSimulationFrame(FPhysicsSimulationParameters& PhysicsSimulationParameters) const
{
	if (PhysicsSimulationParameters.DeltaSeconds <= 0.0f)
	{
		const FPhysicsSimulationParameters DefaultPhysicsSimulationParameters;
		PhysicsSimulationParameters.DeltaSeconds = DefaultPhysicsSimulationParameters.DeltaSeconds;
		LOG_WARNING("DeltaSeconds <= 0.0f. Fallback to default value ({0})", DefaultPhysicsSimulationParameters.DeltaSeconds);
	}

	SimulationWorld->GetPhysicsScene()->SetUpForFrame(
		&PhysicsSimulationParameters.Gravity,
		PhysicsSimulationParameters.DeltaSeconds,
		PhysicsSimulationParameters.MinPhysicsDeltaTime,
		PhysicsSimulationParameters.MaxPhysicsDeltaTime,
		PhysicsSimulationParameters.MaxSubstepDeltaTime,
		PhysicsSimulationParameters.MaxSubsteps,
		PhysicsSimulationParameters.bSubstepping);
}

void UPhysicsWorldSimulationManager::OnStaticActorDestroyed(AActor* StaticActor)
{
	StaticActors.Remove(StaticActor);
}

void UPhysicsWorldSimulationManager::OnPhysicsActorDestroyed(AActor* PhysicsActor)
{
	PhysicsActors.Remove(PhysicsActor);
}
