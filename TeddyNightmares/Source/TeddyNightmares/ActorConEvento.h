// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ActorConEvento.generated.h"

UCLASS()
class TEDDYNIGHTMARES_API AActorConEvento : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AActorConEvento();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UBoxComponent* BoxCollider;

	UFUNCTION()
		void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(BlueprintNativeEvent)
		void EventoEnBlueprint(int32 numero, float valor);
	
	UFUNCTION()
		class UStaticMeshComponent* GetMesh();

	UFUNCTION(BlueprintPure, Category = "Pickup")
		bool IsActive();

	UFUNCTION(BlueprintCallable, Category = "Pickup")
		void SetActive(bool NewState);
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	bool bIsActive;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Pickup", meta = (AllowPrivateAccess = "true"))
		class UStaticMeshComponent* PickupMesh;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
