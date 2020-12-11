// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorConEvento.h"
#include "Components/BoxComponent.h"
#include "TeddyNightmaresCharacter.h"

// Sets default values
AActorConEvento::AActorConEvento()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bIsActive = true;

	PickupMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PickUp Mesh"));
	RootComponent = PickupMesh;
	
	BoxCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollider"));
	//RootComponent = BoxCollider;
	BoxCollider->SetupAttachment(RootComponent);
	
	BoxCollider->OnComponentBeginOverlap.AddDynamic(this, &AActorConEvento::OnBeginOverlap);


}

// Called when the game starts or when spawned
void AActorConEvento::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AActorConEvento::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AActorConEvento::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor != nullptr && OtherActor != this && OtherComp != nullptr && Cast<ATeddyNightmaresCharacter>(OtherActor) != nullptr)
	{
		/*ATeddyNightmaresCharacter* Rufus = Cast<ATeddyNightmaresCharacter>(OtherActor);

		if(Rufus->GetIsPickUp())
		{
			Rufus->AddPickUp();
			Rufus->PrintPickupCount();
			Destroy();
		}*/
		//EventoEnBlueprint(6, 5.3f);
		//Destroy();
	}
}

void AActorConEvento::EventoEnBlueprint_Implementation(int32 numero, float valor)
{
	
}

class UStaticMeshComponent* AActorConEvento::GetMesh()
{
	return PickupMesh;
}


bool AActorConEvento::IsActive()
{
	return bIsActive;
}


void AActorConEvento::SetActive(bool NewState)
{
	bIsActive = NewState;  
}


