// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PickUp.h"
#include "MyCharacterExample.generated.h"

UCLASS()
class TEDDYNIGHTMARES_API AMyCharacterExample : public ACharacter
{
	GENERATED_BODY()

	/** Pawn mesh: 1st person view (arms; seen only by self) */
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
		class USkeletalMeshComponent* Mesh1P;


		/** Location on gun mesh where projectiles should spawn. */
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
		class USceneComponent* FP_MuzzleLocation;


	/** First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* FirstPersonCameraComponent;

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class USpringArmComponent* CameraBoom;

	/** Thirst Person camera follows the character*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* ThirstPersonCameraComponent;

	/** Holding Component */
	UPROPERTY(EditAnywhere)
		class USceneComponent* HoldingComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Collision, meta = (AllowPrivateAccess = "true"))
		class UBoxComponent* CollisionBox;

public:
	// Sets default values for this character's properties
	AMyCharacterExample();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		float BaseLookUpRate;

	/** Gun muzzle's offset from the characters location */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		FVector GunOffset;


	UPROPERTY(EditAnywhere)
		class APickUp* CurrentItem;

	bool bCanMove;
	bool bHoldingItem;
	bool bInspecting;
	bool bIsFirstPersonCameraActive;

	float PitchMax;
	float PitchMin;

	FVector HoldingComp;
	FRotator LastRotation;

	FVector Start;
	FVector ForwardVector;
	FVector End;

	FHitResult Hit;

	FComponentQueryParams DefaultComponentQueryParams;
	FCollisionResponseParams DefaultResponseParam;

protected:

	/** Action Function */
	void OnAction();

	/** Handles moving forward/backward */
	void MoveForward(float Val);

	/** Handles stafing movement, left and right */
	void MoveRight(float Val);

	/**
	 * Called via input to turn at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

	// toggle holding item pickup
	void ToggleItemPickup();

	void ToggleCamera();

private:
	class UAIPerceptionStimuliSourceComponent* Stimulus;

	void InitStimulus();

public:
	/** Returns Mesh1P subobject **/
	FORCEINLINE class USkeletalMeshComponent* GetMesh1P() const { return Mesh1P; }
	/** Returns FirstPersonCameraComponent subobject **/
	FORCEINLINE class UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }

};
