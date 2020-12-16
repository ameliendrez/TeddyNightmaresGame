// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ActorConEvento.h"
#include "Enemy.h"
#include "Components/AudioComponent.h"
#include "Sound/SoundCue.h"
#include "SpawnVolume.generated.h"


UCLASS()
class TEDDYNIGHTMARES_API ASpawnVolume : public AActor
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Audio, meta = (AllowPrivateAccess = "true"))
		class USoundCue* PortalSoundCue;
	
public:	
	// Sets default values for this actor's properties
	ASpawnVolume();


	// FORCEINLINE class UBoxComponent* GetWhereToSpawn() const { return WhereToSpawn; }
	UFUNCTION()
		class UBoxComponent* GetWhereToSpawn();

	UFUNCTION(BlueprintPure, Category="Spawning")
		FVector GetRandomPointInVolume();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = "Spawning")
	TSubclassOf<class ACharacter> WhatToSpawn;

	FTimerHandle SpawnTimer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
	float SpawnDelayRangeLow;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
	float SpawnDelayRangeHigh;
	

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spawning", meta = (AllowPrivateAccess = "true"))
		class UBoxComponent* WhereToSpawn;

	// Handle spawning a new object
	void SpawnEnemy();

	// Current Delay
	float SpawnDelay;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	UAudioComponent* PortalAudioComponent;

};
