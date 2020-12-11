// Fill out your copyright notice in the Description page of Project Settings.


#include "SpawnVolume.h"
#include "Components/BoxComponent.h"
#include "EnemyAIController.h"
#include "Kismet/KismetMathLibrary.h"
#include "ActorConEvento.h"

// Sets default values
ASpawnVolume::ASpawnVolume()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	WhereToSpawn = CreateDefaultSubobject<UBoxComponent>(TEXT("WhereToSpawn"));
	RootComponent = WhereToSpawn;

	// Set the spawn delay

	SpawnDelayRangeLow = 1.0f;
	SpawnDelayRangeHigh = 1.5f;
}

// Called when the game starts or when spawned
void ASpawnVolume::BeginPlay()
{
	Super::BeginPlay();

	SpawnDelay = FMath::FRandRange(SpawnDelayRangeLow, SpawnDelayRangeHigh);
	GetWorldTimerManager().SetTimer(SpawnTimer, this, &ASpawnVolume::SpawnEnemy, SpawnDelay, false);
}

// Called every frame
void ASpawnVolume::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

class UBoxComponent* ASpawnVolume::GetWhereToSpawn()
{
	return WhereToSpawn;
}

FVector ASpawnVolume::GetRandomPointInVolume()
{
	return UKismetMathLibrary::RandomPointInBoundingBox(WhereToSpawn->Bounds.Origin, WhereToSpawn->Bounds.BoxExtent);
}


void ASpawnVolume::SpawnEnemy()
{
	// if we have set something to spawn
	if(WhatToSpawn != NULL)
	{
		// check for a valid world
		UWorld* const World = GetWorld();
		if (World)
		{
			// set the spawn parameters
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this;
			SpawnParams.Instigator = AActor::GetInstigator();


			// Get a ranndom location to spawn at
			FVector SpawnLocation = GetRandomPointInVolume();

			// Get a random rotation for spawned item
			FRotator SpawnRotation;
			SpawnRotation.Yaw = 0.0f;//FMath::FRand() * 360.0f;
			SpawnRotation.Pitch = 0.0f;//FMath::FRand() * 360.0f;
			SpawnRotation.Roll = 0.0f;//FMath::FRand() * 360.0f;
				    
			// spawn the object
			AEnemy* const SpawnedEnemy = World->SpawnActor<AEnemy>(WhatToSpawn, SpawnLocation, SpawnRotation, SpawnParams);

			//SpawnedEnemy->GetMesh()->SetSimulatePhysics(true);
			SpawnedEnemy->GetController()->GetCharacter()->AIControllerClass = AEnemyAIController::StaticClass();


			SpawnDelay = FMath::FRandRange(SpawnDelayRangeLow, SpawnDelayRangeHigh);
			GetWorldTimerManager().SetTimer(SpawnTimer, this, &ASpawnVolume::SpawnEnemy, SpawnDelay, false);
			
		}
	}
}
