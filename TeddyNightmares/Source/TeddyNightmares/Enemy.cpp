// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"
#include "MyCharacterExample.h"
#include "Components/BoxComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
AEnemy::AEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bUseControllerRotationYaw = false;
  
	GetCharacterMovement()->bOrientRotationToMovement = true;
	
	BoxCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollider"));
	BoxCollider->OnComponentBeginOverlap.AddDynamic(this, &AEnemy::OnBeginAttack);

	BoxCollider->OnComponentBeginOverlap.AddDynamic(this, &AEnemy::OnBeginOverlap);

        BoxCollider->AttachTo(GetMesh());
	

}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);



}

// Called to bind functionality to input
void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}


void AEnemy::OnBeginAttack(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor != nullptr && OtherActor != this && OtherComp != nullptr && Cast<AMyCharacterExample>(OtherActor) != nullptr)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, "You lost");
		/*if (montage)
		{
			PlayAnimMontage(montage);
		}*/
	}
}



void AEnemy::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor != nullptr && OtherActor != this && OtherComp != nullptr && Cast<AMyCharacterExample>(OtherActor) != nullptr)
	{



		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, "You lost new");



	}
}

//UAnimMontage* AEnemy::GetMontage() const
//{
//	return montage;
//}