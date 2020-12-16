// Fill out your copyright notice in the Description page of Project Settings.


#include "IsPlayerInRange.h"
#include "EnemyAIController.h"
#include "Enemy.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "blackboard_keys.h"

UIsPlayerInRange::UIsPlayerInRange()
{
	bNotifyBecomeRelevant = true;
	NodeName = TEXT("Is Player In Melee Range");
}

void UIsPlayerInRange::OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::OnBecomeRelevant(OwnerComp, NodeMemory);

	// Get the enemy
	AEnemyAIController* const cont = Cast<AEnemyAIController>(OwnerComp.GetAIOwner());
	AEnemy* const enemy = Cast<AEnemy>(cont->GetPawn());

	// Get player character
	ACharacter* const player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);

	// Write true or false depending on whether the player is within melee_range
	cont->GetBlackboard()->SetValueAsBool(BbKeys::PlayerIsInRange, enemy->GetDistanceTo(player) <= AttackRange);
}
