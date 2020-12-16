// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAttack.h"
#include "AIController.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#include "Engine/LatentActionManager.h"
#include "Components/SkeletalMeshComponent.h"
#include "Animation/AnimMontage.h"
#include "Animation/AnimInstance.h"

UEnemyAttack::UEnemyAttack(FObjectInitializer const& object_initializer)
{
	NodeName = TEXT("Melee Attack");
}

EBTNodeResult::Type UEnemyAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// Get the Enemy
	AAIController* const cont = OwnerComp.GetAIOwner();

	if (AEnemy* const enemy = Cast<AEnemy>(cont->GetPawn()))
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, "You lost new");
		if (MontageHasFinished(enemy))
		{
			enemy->Attack();
		}
	}

	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	return EBTNodeResult::Succeeded;
}

bool UEnemyAttack::MontageHasFinished(AEnemy* const enemy)
{
	return enemy->GetMesh()->GetAnimInstance()->Montage_GetIsStopped(enemy->GetMontage());
}
