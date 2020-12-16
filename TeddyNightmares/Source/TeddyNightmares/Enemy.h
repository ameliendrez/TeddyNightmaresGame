// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Animation/AnimMontage.h"
#include "Components/AudioComponent.h"
#include "Sound/SoundCue.h"
#include "Enemy.generated.h"

UCLASS()
class TEDDYNIGHTMARES_API AEnemy : public ACharacter
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Audio, meta = (AllowPrivateAccess = "true"))
		class USoundCue* EvilDollSoundCue;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Audio, meta = (AllowPrivateAccess = "true"))
		class USoundCue* ChasingSoundCue;
	
public:
	// Sets default values for this character's properties
	AEnemy();

	UAnimMontage* GetMontage() const;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	bool bSeenPlayer;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation", meta = (AllowPrivateAccess = "true"))
		UAnimMontage* montage;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class UBoxComponent* BoxCollider;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Particle System")
		UParticleSystemComponent* ParticleSystem;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void Attack();
	
	UFUNCTION()
		void OnBeginAttack(UPrimitiveComponent* const overlapped_component, AActor* const other_actor, UPrimitiveComponent* other_component, int const other_body_index, bool const from_sweep, FHitResult const& sweep_result);

	UFUNCTION()
		void ToggleFire(bool newState);

	UFUNCTION()
		void setIsSeenPlayer(bool bIsSeenPlayer);

private:
	UAudioComponent* EvilDollAudioComponent;

	UAudioComponent* ChasingAudioComponent;
};
