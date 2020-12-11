// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionTypes.h"
#include "EnemyAIController.generated.h"

class UBehaviorTreeComponent;
class AMyCharacterExample;
struct FAIStimulus;
/**
 * 
 */
UCLASS()
class TEDDYNIGHTMARES_API AEnemyAIController : public AAIController
{
	GENERATED_BODY()

	virtual void OnPossess(class APawn* InPawn) override;

public:
	AEnemyAIController(FObjectInitializer const& object_initializer = FObjectInitializer::Get());

	void BeginPlay() override;
	class UBlackboardComponent* GetBlackboard() const;

private:

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = "true"))
		class UBehaviorTreeComponent* BehaviorTreeComponent;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = "true"))
		class UBehaviorTree* BTree;

	class UBlackboardComponent* Blackboard;

	class UAISenseConfig_Sight* SightConfig;

	UFUNCTION()
		void OnTargetDetected(AActor* actor, FAIStimulus const stimulus);

	void InitPerception();
};
