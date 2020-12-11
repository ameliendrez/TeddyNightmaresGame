// Fill out your copyright notice in the Description page of Project Settings.


#include "MyCharacterExample.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/InputSettings.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "MotionControllerComponent.h"
#include "XRMotionControllerBase.h" // for FXRMotionControllerBase::RightHandSourceId
#include "DrawDebugHelpers.h"
#include "PickUp.h"
#include "GameFramework/SpringArmComponent.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "Perception/AISense_Sight.h"
#include "Components/BoxComponent.h"

//DEFINE_LOG_CATEGORY_STATIC(LogFPChar, Warning, All);

// Sets default values
AMyCharacterExample::AMyCharacterExample()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	//PrimaryActorTick.bCanEverTick = true;

	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);

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

	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-39.56f, 1.75f, 64.f)); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;
	FirstPersonCameraComponent->bAutoActivate = false;

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(FirstPersonCameraComponent);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	Mesh1P->SetRelativeRotation(FRotator(1.5f, -15.0f, 0.0f));
	Mesh1P->SetRelativeLocation(FVector(20.0f, 0.0f, -50.0f));

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	ThirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("ThirstPersonCamera"));
	ThirstPersonCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	ThirstPersonCameraComponent->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	FP_MuzzleLocation = CreateDefaultSubobject<USceneComponent>(TEXT("MuzzleLocation"));
	FP_MuzzleLocation->SetupAttachment(Mesh1P, TEXT("GripPoint"));
	FP_MuzzleLocation->SetRelativeLocation(FVector(30.0f, 30.0f, 25.0f));

	HoldingComponent = CreateDefaultSubobject<USceneComponent>(TEXT("HoldingComponent"));
	HoldingComponent->GetRelativeLocation().SetComponentForAxis(EAxis::X, 5.0f);
	HoldingComponent->SetupAttachment(FP_MuzzleLocation);

	CurrentItem = NULL;
	bCanMove = true;
	//bInspecting = false;
	bIsFirstPersonCameraActive = true;

	InitStimulus();

	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));

	if (CollisionBox)
	{
		FVector const extent(8.0f);
		CollisionBox->SetBoxExtent(extent, false);
		CollisionBox->SetCollisionProfileName("NoCollision");
	}

}

// Called when the game starts or when spawned
void AMyCharacterExample::BeginPlay()
{
	Super::BeginPlay();

	PitchMax = GetWorld()->GetFirstPlayerController()->PlayerCameraManager->ViewPitchMax;
	PitchMin = GetWorld()->GetFirstPlayerController()->PlayerCameraManager->ViewPitchMin;

	if (CollisionBox)
	{
		FAttachmentTransformRules const rules(
			EAttachmentRule::SnapToTarget,
			EAttachmentRule::SnapToTarget,
			EAttachmentRule::KeepWorld,
			false);

		CollisionBox->AttachToComponent(GetMesh(), rules, "hand_r_socket");
		CollisionBox->SetRelativeLocation(FVector(-7.0f, 0.0f, 0.0f));
	}
}

// Called every frame
void AMyCharacterExample::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	Start = FirstPersonCameraComponent->GetComponentLocation();
	ForwardVector = FirstPersonCameraComponent->GetForwardVector();
	End = ((ForwardVector * 200.f) + Start);

	//DrawDebugLine(GetWorld(), Start, End, FColor::Green, false, 1, 0, 1);

	if (!bHoldingItem)
	{
		if (GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, DefaultComponentQueryParams, DefaultResponseParam))
		{
			if (Hit.GetActor()->GetClass()->IsChildOf(APickUp::StaticClass()))
			{
				CurrentItem = Cast<APickUp>(Hit.GetActor());
			}
		}
		else
		{
			CurrentItem = NULL;
		}
	}

	if(bIsFirstPersonCameraActive)
	{
		FirstPersonCameraComponent->SetFieldOfView(FMath::Lerp(FirstPersonCameraComponent->FieldOfView, 90.0f, 0.1f));
	}

	if (bHoldingItem)
	{
		const float CubeRelativePositionX = (bIsFirstPersonCameraActive) ? 15.0f : -25.0f;
		const float CubeRelativePositionY = (bIsFirstPersonCameraActive) ? 0.0f : -28.0f;
		const float CubeRelativePositionZ = (bIsFirstPersonCameraActive) ? -30.0f : -45.0f;
		HoldingComponent->SetRelativeLocation(FVector(CubeRelativePositionX, CubeRelativePositionY, CubeRelativePositionZ));
	}
	
}

// Called to bind functionality to input
void AMyCharacterExample::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// set up gameplay key bindings
	check(PlayerInputComponent);

	// Bind action event
	PlayerInputComponent->BindAction("Action", IE_Pressed, this, &AMyCharacterExample::OnAction);
	PlayerInputComponent->BindAction("ToggleCamera", IE_Pressed, this, &AMyCharacterExample::ToggleCamera);

	// Bind movement events
	PlayerInputComponent->BindAxis("MoveForward", this, &AMyCharacterExample::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AMyCharacterExample::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &AMyCharacterExample::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AMyCharacterExample::LookUpAtRate);
}


void AMyCharacterExample::MoveForward(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void AMyCharacterExample::MoveRight(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f))
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

void AMyCharacterExample::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AMyCharacterExample::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}


void AMyCharacterExample::OnAction()
{
	if (CurrentItem)
	{
		ToggleItemPickup();
	}
}


void AMyCharacterExample::ToggleItemPickup()
{
	if (CurrentItem)
	{
		bHoldingItem = !bHoldingItem;
		CurrentItem->PickUp();

		if (!bHoldingItem)
		{
			CurrentItem = NULL;
		}
	}
}

void AMyCharacterExample::ToggleCamera()
{
	if(bIsFirstPersonCameraActive)
	{
		FirstPersonCameraComponent->SetActive(false);
		ThirstPersonCameraComponent->SetActive(true);
		this->bUseControllerRotationYaw = false;
		bIsFirstPersonCameraActive = false;
	} else
	{
		FirstPersonCameraComponent->SetActive(true);
		ThirstPersonCameraComponent->SetActive(false);
		this->bUseControllerRotationYaw = true;
		bIsFirstPersonCameraActive = true;
	}
}

void AMyCharacterExample::InitStimulus()
{
	// Create stimulus
	Stimulus = CreateDefaultSubobject<UAIPerceptionStimuliSourceComponent>(TEXT("stimulus"));
	Stimulus->RegisterForSense(TSubclassOf<UAISense_Sight>());
	Stimulus->RegisterWithPerceptionSystem();
}