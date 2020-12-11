// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAIController.h"
#include "blackboard_keys.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "MyCharacterExample.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "blackboard_keys.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#include "GameFramework/Character.h"

AEnemyAIController::AEnemyAIController(FObjectInitializer const& object_initializer)
{
	static ConstructorHelpers::FObjectFinder<UBehaviorTree> obj(TEXT("BehaviorTree'/Game/Doll/IA/Doll_BT.Doll_BT'"));
	if (obj.Succeeded())
	{
		BTree = obj.Object;
	}
	BehaviorTreeComponent = object_initializer.CreateDefaultSubobject<UBehaviorTreeComponent>(this, TEXT("BehaviorTreeComponent"));
	Blackboard = object_initializer.CreateDefaultSubobject<UBlackboardComponent>(this, TEXT("BlackboardComp"));

	InitPerception();
}


void AEnemyAIController::BeginPlay()
{
	Super::BeginPlay();
	RunBehaviorTree(BTree);
	BehaviorTreeComponent->StartTree(*BTree);
}

void AEnemyAIController::OnPossess(APawn* const pawn)
{
	Super::OnPossess(pawn);
	if (Blackboard)
	{
		Blackboard->InitializeBlackboard(*BTree->BlackboardAsset);
	}
}

UBlackboardComponent* AEnemyAIController::GetBlackboard() const
{
	return Blackboard;
}

void AEnemyAIController::OnTargetDetected(AActor* actor, FAIStimulus const stimulus)
{
	if (auto const ch = Cast<AMyCharacterExample>(actor))
	{
		GetBlackboard()->SetValueAsBool(BbKeys::PlayerVisible, stimulus.WasSuccessfullySensed());
	}
}


void AEnemyAIController::InitPerception()
{
	//Create and initialize sight config object
	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Config"));
	SetPerceptionComponent(*CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("Perception Component")));
	SightConfig->SightRadius = 500.0f;
	// Enemy won't be able to see player
	SightConfig->LoseSightRadius = SightConfig->SightRadius + 50.0f;
	// Field of view
	SightConfig->PeripheralVisionAngleDegrees = 90.0f;
	//Time after chase stimulus disappear
	SightConfig->SetMaxAge(5.0f);
	// Defines how close the source has to stay close to the last location
	SightConfig->AutoSuccessRangeFromLastSeenLocation = 900.0f;
	SightConfig->DetectionByAffiliation.bDetectEnemies = true;
	SightConfig->DetectionByAffiliation.bDetectFriendlies = true;
	SightConfig->DetectionByAffiliation.bDetectNeutrals = true;

	// Add sight config comp to perception comp
	GetPerceptionComponent()->SetDominantSense(*SightConfig->GetSenseImplementation());
	GetPerceptionComponent()->OnTargetPerceptionUpdated.AddDynamic(this, &AEnemyAIController::OnTargetDetected);
	GetPerceptionComponent()->ConfigureSense(*SightConfig);
}

