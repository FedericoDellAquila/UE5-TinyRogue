#pragma once
#include "CoreMinimal.h"
#include "Card.h"
#include "DungeonLevelsDataTableRow.h"
#include "MonsterCard.generated.h"

class UHealthComponent;

UCLASS()
class TINYROGUE_API AMonsterCard : public ACard
{
	GENERATED_BODY()

public:
	AMonsterCard();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UHealthComponent> HealthComponent;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FEnemyData EnemyData;
};
