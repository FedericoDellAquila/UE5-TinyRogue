#include "TinyRogue/Public/Core/TinyRogueGameMode.h"
#include "Core/DungeonLevelManagerComponent.h"
#include "Core/GameStateMachine/StateMachineComponent.h"
#include "Core/TimerTracker.h"
#include "Core/RollManager/RollManagerComponent.h"
#include "GameFramework/GameMode.h"
#include "Utility/Log.h"

ATinyRogueGameMode::ATinyRogueGameMode()
{
	DungeonLevelManager = CreateDefaultSubobject<UDungeonLevelManagerComponent>(TEXT("DungeonLevelManager"));
}

void ATinyRogueGameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);

	if (IsValid(RollManager) == false)
		RollManager = Cast<URollManagerComponent>(GetComponentByClass(URollManagerComponent::StaticClass()));
	
	CLOG_FATAL(IsValid(RollManager) == false, "RollManager is nullptr.");

	if (IsValid(GameStateMachine) == false)
		GameStateMachine = Cast<UStateMachineComponent>(GetComponentByClass(UStateMachineComponent::StaticClass()));
	
	CLOG_FATAL(IsValid(GameStateMachine) == false, "GameStateMachine is nullptr.");
	
	if (IsValid(TimeTracker) == false)
		TimeTracker = NewObject<UTimeTracker>(this);
	
	CLOG_FATAL(IsValid(TimeTracker) == false, "TimeTracker is nullptr.");

	DungeonLevelManager->Initialize();
}

void ATinyRogueGameMode::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	PostInitGame();
}

bool ATinyRogueGameMode::SetPause(APlayerController* PC, FCanUnpause CanUnpauseDelegate)
{
	
	
	return Super::SetPause(PC, CanUnpauseDelegate);
}
