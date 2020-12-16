// Fill out your copyright notice in the Description page of Project Settings.


#include "WaitingToAttack.h"
#include "AIController.h"
#include "Enemy.h"

UWaitingToAttack::UWaitingToAttack(FObjectInitializer const& object_initializer)
{
	NodeName = TEXT("Waiting to Attack");
}

EBTNodeResult::Type UWaitingToAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// Get the Enemy
	AAIController* const cont = OwnerComp.GetAIOwner();

	if (AEnemy* const enemy = Cast<AEnemy>(cont->GetPawn()))
	{
		enemy->ToggleFire(false);
		enemy->setIsSeenPlayer(false);
	}

	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	return EBTNodeResult::Succeeded;
}