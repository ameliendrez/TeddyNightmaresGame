// Fill out your copyright notice in the Description page of Project Settings.


#include "ChasePlayer.h"
#include "EnemyAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "blackboard_keys.h"

UChasePlayer::UChasePlayer(FObjectInitializer const& object_initializer)
{
	NodeName = TEXT("Chase Player");
}

EBTNodeResult::Type UChasePlayer::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// Get TargetLocation from blackboard via AI Controller
	AEnemyAIController* const cont = Cast<AEnemyAIController>(OwnerComp.GetAIOwner());
	FVector const PlayerLocation = cont->GetBlackboard()->GetValueAsVector(BbKeys::TargetLocation);

	//Move to the player's location
	UAIBlueprintHelperLibrary::SimpleMoveToLocation(cont, PlayerLocation);

	//Finish with success
	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	return EBTNodeResult::Succeeded;
}
