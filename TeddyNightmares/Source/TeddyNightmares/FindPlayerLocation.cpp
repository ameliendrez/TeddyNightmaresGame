// Fill out your copyright notice in the Description page of Project Settings.


#include "FindPlayerLocation.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Runtime/NavigationSystem/Public/NavigationSystem.h"
#include "EnemyAIController.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "blackboard_keys.h"

UFindPlayerLocation::UFindPlayerLocation(FObjectInitializer const& object_initializer)
{
	NodeName = TEXT("Find Player Location");
}

EBTNodeResult::Type UFindPlayerLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// Get player character and the Enemy's controller
	ACharacter* const Player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	auto const cont = Cast<AEnemyAIController>(OwnerComp.GetAIOwner());

	// Get player location to use as an origin
	FVector const PlayerLocation = Player->GetActorLocation();

	if (bSearchRandom)
	{
		FNavLocation Location;

		// Get the nav system and generate a random location near the player
		UNavigationSystemV1* const NavSystem = UNavigationSystemV1::GetCurrent(GetWorld());
		if (NavSystem->GetRandomPointInNavigableRadius(PlayerLocation, SearchRadius, Location, nullptr))
		{
			cont->GetBlackboard()->SetValueAsVector(BbKeys::TargetLocation, Location.Location);
		}
	}
	else
	{
		cont->GetBlackboard()->SetValueAsVector(BbKeys::TargetLocation, PlayerLocation);
	}

	// Finish with success
	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);

	return EBTNodeResult::Succeeded;
}