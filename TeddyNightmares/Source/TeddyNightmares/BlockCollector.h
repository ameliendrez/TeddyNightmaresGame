// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BlockCollector.generated.h"

UCLASS()
class TEDDYNIGHTMARES_API ABlockCollector : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABlockCollector();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class UBoxComponent* BoxCollider;

	UPROPERTY(VisibleAnywhere)
		class UStaticMeshComponent* BlockCollectorMesh;

	UPROPERTY(VisibleAnywhere)
		int32 QuantityBlocks;

	UPROPERTY(VisibleAnywhere)
		int32 TotalBlocks;
	
	UFUNCTION()
		void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

};
