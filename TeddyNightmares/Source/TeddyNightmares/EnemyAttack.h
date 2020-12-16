// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "Enemy.h"
#include "EnemyAttack.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class TEDDYNIGHTMARES_API UEnemyAttack : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:
	UEnemyAttack(FObjectInitializer const& object_initializer);

	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:
	bool MontageHasFinished(AEnemy* const enemy);
};
