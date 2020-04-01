// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "SneakCharacter.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "Components/SphereComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "Animation/AnimInstance.h"
#include "PickUp.h"
#include "Projectile.h"


//////////////////////////////////////////////////////////////////////////
// ASneakCharacter

ASneakCharacter::ASneakCharacter()
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
	GetCharacterMovement()->JumpZVelocity = 100.f;
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

	//Create a ariel camera
	WeaponCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("ArielCamera"));
	//WeaponCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	WeaponCamera->AttachToComponent(GetMesh(), FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("WeaponCam"));
	WeaponCamera->bUsePawnControlRotation = true; // Camera does rotate relative to arm

	//Create the Collection Sphere
	CollectionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("CollectionSphere"));
	CollectionSphere->SetupAttachment(RootComponent);
	CollectionSphere->SetSphereRadius(100.f);

	//Create the Weapon
	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
	WeaponMesh->AttachToComponent(GetMesh(), FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("WeaponHolder"));

	//Create the muzzle location
	MuzzleLocation = CreateDefaultSubobject<USceneComponent>(TEXT("MuzzleLocation"));
	MuzzleLocation->SetupAttachment(WeaponMesh);
	MuzzleLocation->SetRelativeLocation(FVector());

	// Default offset from the character location for projectiles to spawn
	GunOffset = FVector(0.0f, 0.0f, 0.0f);

	WasCollected = false;
	HasWon = false;

	MaxAmmo = 3;
	Ammo = MaxAmmo;

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)

}

//////////////////////////////////////////////////////////////////////////
// Input

void ASneakCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &ASneakCharacter::Sprinting);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &ASneakCharacter::SetNormalSpeed);

	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &ASneakCharacter::FireWeapon);
	

	PlayerInputComponent->BindAction("Collect", IE_Pressed, this, &ASneakCharacter::CollectPickUps);

	PlayerInputComponent->BindAxis("MoveForward", this, &ASneakCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ASneakCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &ASneakCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &ASneakCharacter::LookUpAtRate);

	// handle touch devices
	PlayerInputComponent->BindTouch(IE_Pressed, this, &ASneakCharacter::TouchStarted);
	PlayerInputComponent->BindTouch(IE_Released, this, &ASneakCharacter::TouchStopped);

	// VR headset functionality
	PlayerInputComponent->BindAction("ResetVR", IE_Pressed, this, &ASneakCharacter::OnResetVR);
}


void ASneakCharacter::OnResetVR()
{
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

void ASneakCharacter::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
		Jump();
}

void ASneakCharacter::TouchStopped(ETouchIndex::Type FingerIndex, FVector Location)
{
		StopJumping();
}

void ASneakCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void ASneakCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void ASneakCharacter::MoveForward(float Value)
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

void ASneakCharacter::MoveRight(float Value)
{
	if ( (Controller != NULL) && (Value != 0.0f) )
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

void ASneakCharacter::SetCharacterSpeed(float Speed)
{	
	//sets the player speed to the inputted speed
	GetCharacterMovement()->MaxWalkSpeed = Speed;
}

void ASneakCharacter::SetCharacterCrouchSpeed(float Speed)
{
	//Sets the player's crouched speed to the inputted speed
	GetCharacterMovement()->MaxWalkSpeedCrouched = Speed;
}

void ASneakCharacter::Sprinting()
{
	SetCharacterSpeed(600.f);
}

void ASneakCharacter::SetNormalSpeed()
{
	SetCharacterSpeed(300.f);
}

void ASneakCharacter::ChangeCamera(bool CameraSwitch)
{
	WeaponCamera->bIsActive = CameraSwitch;
	FollowCamera->bIsActive = !WeaponCamera->bIsActive;
}

int ASneakCharacter::GetAmmo()
{
	return Ammo;
}

void ASneakCharacter::Equip()
{
	UE_LOG(LogTemp, Warning, TEXT("Weapon Equipped"));
	//Set the character Speed to 0
	SetCharacterSpeed(0.0f);
	//Change the camera to the weapon camera
	ChangeCamera(true);
	//Bool to see if the weapon is equipped
	IsEquipped = true;
	//Attach the weapon to the players hand
	WeaponMesh->AttachToComponent(GetMesh(), FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("Weapon"));
	//Rotates the player with the camera
	bUseControllerRotationYaw = true;
}

void ASneakCharacter::UnEquip()
{
	UE_LOG(LogTemp, Warning, TEXT("Weapon UnEquipped"));
	//Set the character Speed to normal
	SetNormalSpeed();
	//Change the camera to the orignal camera
	ChangeCamera(false);
	//Bool to see if the weapon is equipped
	IsEquipped = false;
	//Attach the weapon to the players back
	WeaponMesh->AttachToComponent(GetMesh(), FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("WeaponHolder"));
	//Allows the player to rotate without rotating the player
	bUseControllerRotationYaw = false;
}

void ASneakCharacter::FireWeapon()
{
	if (IsEquipped == true)
	{
		if (Ammo >= 1)
		{
			if (ProjectileClass != NULL)
			{
				UWorld* const World = GetWorld();
				if (World != NULL)
				{
				const FRotator SpawnRotation = MuzzleLocation->GetComponentRotation();
				const FVector SpawnLocation = ((MuzzleLocation != nullptr) ? MuzzleLocation->GetComponentLocation() : GetActorLocation()) + SpawnRotation.RotateVector(GunOffset);

				//Set SpawnCollision Handling Override
				FActorSpawnParameters ActorSpawnParams;
				ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;
				World->SpawnActor<AProjectile>(ProjectileClass, SpawnLocation, SpawnRotation, ActorSpawnParams);

				Ammo--;
				}
			}
		}
		
		
	}
}

void ASneakCharacter::CollectPickUps()
{
	//Get all overlapping Actors and store them in an array
	TArray<AActor*> CollectedActors;
	CollectionSphere->GetOverlappingActors(CollectedActors);

	//For each actor we collect
	for (int32 iCollected = 0; iCollected < CollectedActors.Num(); iCollected++)
	{
		//cast the actor to APickup
		APickUp* const Pickup = Cast<APickUp>(CollectedActors[iCollected]);
		
		//if the cast is successful and the pickup is valid and active
		if (Pickup && !Pickup->IsPendingKill() && Pickup->IsActive())
		{
			//call the pickup wascollected function
			Pickup->WasCollected();
			//Deactivate the pickup
			Pickup->SetActive(false);

			WasCollected = true;
		}
		
	}
	
}
