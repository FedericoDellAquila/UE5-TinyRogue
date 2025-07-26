#pragma once
#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "TinyRoguePlayerController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGamePausedDelegate, const bool, bIsPaused);

class UStateMachineComponent;

UCLASS()
class TINYROGUE_API ATinyRoguePlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ATinyRoguePlayerController();

	UPROPERTY(BlueprintAssignable)
	FOnGamePausedDelegate OnGamePausedDelegate;
	
	virtual bool SetPause(bool bPause, FCanUnpause CanUnpauseDelegate = FCanUnpause()) override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UStateMachineComponent> InputStateMachine;
};
