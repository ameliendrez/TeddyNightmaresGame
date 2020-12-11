// Copyright Epic Games, Inc. All Rights Reserved.

#include "TeddyNightmaresCharacter.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/SpotLightComponent.h"
#include "DrawDebugHelpers.h"

//////////////////////////////////////////////////////////////////////////
// ATeddyNightmaresCharacter

ATeddyNightmaresCharacter::ATeddyNightmaresCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)
	//Lintern = CreateDefaultSubobject<USpotLightComponent>(TEXT("Lintern"));
	//Lintern->SetupAttachment(RootComponent);
	//Lintern->SetIntensity(1000.f);

	//bIsPressedPickUp = false;
	//PickUpCount = 0;

	HoldingComponent = CreateDefaultSubobject<USceneComponent>(TEXT("HoldingComponent"));
	HoldingComponent->GetRelativeLocation().SetComponentForAxis(EAxis::X, 50.0f);
	HoldingComponent->SetupAttachment(RootComponent); //FP_MUZZLE COMPONENT --> GUN

	CurrentItem = NULL;
	bCanMove = true;
	bInspecting = false;
}

void ATeddyNightmaresCharacter::BeginPlay()
{
	Super::BeginPlay();

	PitchMax = GetWorld()->GetFirstPlayerController()->PlayerCameraManager->ViewPitchMax;
	PitchMin = GetWorld()->GetFirstPlayerController()->PlayerCameraManager->ViewPitchMin;
}


void ATeddyNightmaresCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	Start = FollowCamera->GetComponentLocation();
	ForwardVector = FollowCamera->GetForwardVector();
	End = ((ForwardVector * 200.0f) + Start);

	DrawDebugLine(GetWorld(), Start, End, FColor::Green, false, 1, 0, 1);

	if(!bHoldingItem)
	{
		if(GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, DefaultComponentQueryParams
		, DefaultResponseParams))
		{
			if(Hit.GetActor()->GetClass()->IsChildOf(APickUp::StaticClass()))
			{
				CurrentItem = Cast<APickUp>(Hit.GetActor());
			}
		}
	} else
	{
		CurrentItem = NULL;	
	}

	if(bInspecting)
	{
		if(bHoldingItem)
		{
			FollowCamera->SetFieldOfView(FMath::Lerp(FollowCamera->FieldOfView, 90.0f, 0.1f));
			HoldingComponent->SetRelativeLocation(FVector(0.0f, 50.0f, 50.0f));
			GetWorld()->GetFirstPlayerController()->PlayerCameraManager->ViewPitchMax = 179.90000002f;
			GetWorld()->GetFirstPlayerController()->PlayerCameraManager->ViewPitchMin = -179.90000002f;
			CurrentItem->RotateActor();
		} else
		{
			FollowCamera->SetFieldOfView(FMath::Lerp(FollowCamera->FieldOfView, 45.0f, 0.1f));
		}
	} else
	{
		FollowCamera->SetFieldOfView(FMath::Lerp(FollowCamera->FieldOfView, 90.0f, 0.1f));

		if(bHoldingItem)
		{
			HoldingComponent->SetRelativeLocation(FVector(50.0f, 0.0f, 0.0f));
		}
	}
}


//////////////////////////////////////////////////////////////////////////
// Input

void ATeddyNightmaresCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &ATeddyNightmaresCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ATeddyNightmaresCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &ATeddyNightmaresCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &ATeddyNightmaresCharacter::LookUpAtRate);

	// handle touch devices
	//PlayerInputComponent->BindTouch(IE_Pressed, this, &ATeddyNightmaresCharacter::TouchStarted);
	//PlayerInputComponent->BindTouch(IE_Released, this, &ATeddyNightmaresCharacter::TouchStopped);

	// VR headset functionality
	//PlayerInputComponent->BindAction("ResetVR", IE_Pressed, this, &ATeddyNightmaresCharacter::OnResetVR);

	//PlayerInputComponent->BindAction("TurnOnLight", IE_Pressed, this, &ATeddyNightmaresCharacter::TurnOnLintern);
	//PlayerInputComponent->BindAction("TurnOnLight", IE_Released, this, &ATeddyNightmaresCharacter::StopPress);

	PlayerInputComponent->BindAction("Action", IE_Pressed, this, &ATeddyNightmaresCharacter::OnAction);
	PlayerInputComponent->BindAction("Inspect", IE_Pressed, this, &ATeddyNightmaresCharacter::OnInspect);
	PlayerInputComponent->BindAction("Inspect", IE_Released, this, &ATeddyNightmaresCharacter::OnInspectReleased);
}


/*void ATeddyNightmaresCharacter::OnResetVR()
{
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

void ATeddyNightmaresCharacter::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
		Jump();
}

void ATeddyNightmaresCharacter::TouchStopped(ETouchIndex::Type FingerIndex, FVector Location)
{
		StopJumping();
}*/

void ATeddyNightmaresCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void ATeddyNightmaresCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void ATeddyNightmaresCharacter::MoveForward(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f) && bCanMove)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void ATeddyNightmaresCharacter::MoveRight(float Value)
{
	if ( (Controller != NULL) && (Value != 0.0f) && bCanMove)
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
	
		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}

/*void ATeddyNightmaresCharacter::TurnOnLintern()
{
	Lintern->SetVisibility(!Lintern->GetVisibleFlag());
	bIsPressedPickUp = true;
}

void ATeddyNightmaresCharacter::StopPress()
{
	bIsPressedPickUp = false;
}

bool ATeddyNightmaresCharacter::GetIsPickUp()
{
	return bIsPressedPickUp;
}

void ATeddyNightmaresCharacter::AddPickUp()
{
	PickUpCount = PickUpCount + 1;
}

int32 ATeddyNightmaresCharacter::GetPickupCount()
{
	return this->PickUpCount;
}

void ATeddyNightmaresCharacter::PrintPickupCount() {
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::FromInt(PickUpCount));
}*/


void ATeddyNightmaresCharacter::OnAction()
{
	if(CurrentItem && !bInspecting)
	{
		ToggleItemPickup();
	}
}

void ATeddyNightmaresCharacter::OnInspect()
{
	if(bHoldingItem)
	{
		LastRotation = GetControlRotation();
		ToggleMovement();
	} else
	{
		bInspecting = true;
	}
}

void ATeddyNightmaresCharacter::OnInspectReleased()
{
	if(bInspecting && bHoldingItem)
	{
		GetController()->SetControlRotation(LastRotation);
		GetWorld()->GetFirstPlayerController()->PlayerCameraManager->ViewPitchMax = PitchMax;
		GetWorld()->GetFirstPlayerController()->PlayerCameraManager->ViewPitchMin = PitchMin;
		ToggleMovement();
	} else
	{
		bInspecting = false;
	}
}

void ATeddyNightmaresCharacter::ToggleMovement()
{
	bCanMove = !bCanMove;
	bInspecting = !bInspecting;
	FollowCamera->bUsePawnControlRotation = !FollowCamera->bUsePawnControlRotation;
	bUseControllerRotationYaw = !bUseControllerRotationYaw;
}

void ATeddyNightmaresCharacter::ToggleItemPickup()
{
	if(CurrentItem)
	{
		bHoldingItem = !bHoldingItem;
		CurrentItem->PickUp();

		if(!bHoldingItem)
		{
			CurrentItem = NULL;
		}
	}
}
