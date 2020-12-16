// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"
#include "MyCharacterExample.h"
#include "Components/BoxComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"

// Sets default values
AEnemy::AEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bUseControllerRotationYaw = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	
	BoxCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollider"));
	//BoxCollider->SetSimulatePhysics(true);

	if (BoxCollider)
	{
		FVector const extent(8.0f);
		BoxCollider->SetBoxExtent(extent, false);
		BoxCollider->SetCollisionProfileName("NoCollision");
	}
	
	BoxCollider->OnComponentBeginOverlap.AddDynamic(this, &AEnemy::OnBeginAttack);


	ParticleSystem = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ParticleSystem"));
	ParticleSystem->SetVisibility(false);
	ParticleSystem->SetupAttachment(RootComponent);

	static ConstructorHelpers::FObjectFinder<USoundCue> EvilDollSoundCueObject(TEXT("SoundCue'/Game/Audio/EvilDollCue.EvilDollCue'"));
	static ConstructorHelpers::FObjectFinder<USoundCue> ChasingSoundCueObject(TEXT("SoundCue'/Game/Audio/AtmospherePersecutionCue.AtmospherePersecutionCue'"));

	
	if (EvilDollSoundCueObject.Succeeded()) {
		EvilDollSoundCue = EvilDollSoundCueObject.Object;

		EvilDollAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("EvilDollAudioComponent"));
		EvilDollAudioComponent->SetupAttachment(RootComponent);
		
	}

	if (ChasingSoundCueObject.Succeeded()) {
		ChasingSoundCue = ChasingSoundCueObject.Object;

		ChasingAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("ChasingAudioComponent"));
		ChasingAudioComponent->SetupAttachment(RootComponent);
	}

	bSeenPlayer = false;
}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();

	if (BoxCollider)
	{
		FAttachmentTransformRules const rules(
			EAttachmentRule::SnapToTarget,
			EAttachmentRule::SnapToTarget,
			EAttachmentRule::KeepWorld,
			false);

		BoxCollider->AttachToComponent(GetMesh(), rules, "hand_r_socket");
		BoxCollider->SetRelativeLocation(FVector(-7.0f, 0.0f, 0.0f));
		BoxCollider->OnComponentBeginOverlap.AddDynamic(this, &AEnemy::OnBeginAttack);
	}

	if (EvilDollAudioComponent && EvilDollSoundCue) {
		EvilDollAudioComponent->SetSound(EvilDollSoundCue);
		EvilDollAudioComponent->Stop();
	}

	if (ChasingAudioComponent && ChasingSoundCue) {
		ChasingAudioComponent->SetSound(ChasingSoundCue);
		ChasingAudioComponent->Stop();
	}
}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bSeenPlayer && ChasingAudioComponent && ChasingSoundCue && !ChasingAudioComponent->IsPlaying()) {
		ChasingAudioComponent->Play(0.f);
	}

	if (!bSeenPlayer && ChasingAudioComponent && ChasingSoundCue && ChasingAudioComponent->IsPlaying()) {
		ChasingAudioComponent->Stop();
	}
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
		//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, "You lost");
		if (montage)
		{
			Cast<AMyCharacterExample>(OtherActor)->Kill();
			PlayAnimMontage(montage);
		}
	}
}

void AEnemy::Attack()
{
	if (montage)
	{
		PlayAnimMontage(montage);
	}
}


UAnimMontage* AEnemy::GetMontage() const
{
	return montage;
}


void AEnemy::ToggleFire(bool newState)
{
	ParticleSystem->SetVisibility(newState);

	if (newState && EvilDollAudioComponent && EvilDollSoundCue && !EvilDollAudioComponent->IsPlaying()) {
		EvilDollAudioComponent->Play(0.f);
	}
}

void AEnemy::setIsSeenPlayer(bool bIsSeenPlayer)
{
	bSeenPlayer = bIsSeenPlayer;
}