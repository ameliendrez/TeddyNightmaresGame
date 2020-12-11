// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PickUp.h"
#include "TeddyNightmaresCharacter.generated.h"

UCLASS(config=Game)
class ATeddyNightmaresCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Componentes, meta = (AllowPrivateAccess = "true"))
	//class USpotLightComponent* Lintern;

	UPROPERTY(EditAnywhere)
		class USceneComponent* HoldingComponent;
	
	
public:
	ATeddyNightmaresCharacter();

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;

	//UFUNCTION()
	//void TurnOnLintern();


	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Spawning)
		//bool bIsPressedPickUp;

	//UFUNCTION()
		//void StopPress();


	//UFUNCTION(Category = Spawning)
		//bool GetIsPickUp();

	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		//int32 PickUpCount;

	//UFUNCTION()
		//void AddPickUp();

	//UFUNCTION()
		//int32 GetPickupCount();

	//UFUNCTION()
		//void PrintPickupCount();

protected:
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;
	
	/** Resets HMD orientation in VR. */
	//void OnResetVR();

	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);

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

	/** Handler for when a touch input begins. */
	//void TouchStarted(ETouchIndex::Type FingerIndex, FVector Location);

	/** Handler for when a touch input stops. */
	//void TouchStopped(ETouchIndex::Type FingerIndex, FVector Location);

public:
	UPROPERTY(EditAnywhere)
		class APickUp* CurrentItem;

	bool bCanMove;
	bool bHoldingItem;
	bool bInspecting;

	float PitchMax;
	float PitchMin;

	FVector HoldingComp;
	FRotator LastRotation;
	
	FVector Start;
	FVector ForwardVector;
	FVector End;

	FHitResult Hit;

	FComponentQueryParams DefaultComponentQueryParams;
	FCollisionResponseParams DefaultResponseParams;

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// End of APawn interface

	void OnAction();
	void OnInspect();
	void OnInspectReleased();

	void ToggleMovement();
	void ToggleItemPickup();

	
public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
};

