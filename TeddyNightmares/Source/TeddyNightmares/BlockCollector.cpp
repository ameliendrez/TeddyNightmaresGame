// Fill out your copyright notice in the Description page of Project Settings.


#include "BlockCollector.h"
#include "Components/BoxComponent.h"
#include "PickUp.h"

// Sets default values
ABlockCollector::ABlockCollector()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	QuantityBlocks = 0;

	TotalBlocks = 5;

	BlockCollectorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BlockCollectorMesh"));
	RootComponent = BlockCollectorMesh;

	BoxCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollider"));
	BoxCollider->SetupAttachment(RootComponent);

	BoxCollider->OnComponentBeginOverlap.AddDynamic(this, &ABlockCollector::OnBeginOverlap);
}

// Called when the game starts or when spawned
void ABlockCollector::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABlockCollector::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABlockCollector::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor != nullptr && OtherActor != this && OtherComp != nullptr && Cast<APickUp>(OtherActor) != nullptr)
	{
		QuantityBlocks++;
		
		APickUp* block = Cast<APickUp>(OtherActor);
		block->DestroyBlock();

		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::FromInt(TotalBlocks - QuantityBlocks) + " blocks lefts");
		
		if(QuantityBlocks == TotalBlocks)
		{
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, "You win");
		}

	}
}

