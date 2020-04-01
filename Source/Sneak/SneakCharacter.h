// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SneakCharacter.generated.h"

UCLASS(config=Game)
class ASneakCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* WeaponCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon", meta = (AllowPrivateAccess = "true"))
	class USkeletalMeshComponent* WeaponMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CollectionSpehre", meta = (AllowPrivateAccess = "true"))
	class USphereComponent* CollectionSphere;
public:
	ASneakCharacter();

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;

	//Ammo counter
	int Ammo;
	//Max Ammo Counter
	int MaxAmmo;

	//Bool to check if the collectible has been collected
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool WasCollected;

	//Bool to check if the player has won
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool HasWon;

	//Bool to check if the weapon is equipped
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool IsEquipped;

	//Equip the pistol
	UFUNCTION(BlueprintCallable)
		void Equip();
	//Unequip the pistol
	UFUNCTION(BlueprintCallable)
		void UnEquip();

	//Get the amount of ammo
	UFUNCTION(BlueprintCallable)
		int GetAmmo();
	
	//Muzzle Location to spawn the players projectile
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
		class USceneComponent* MuzzleLocation;

	/** Gun muzzle's offset from the characters location */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		FVector GunOffset;

	/** Projectile class to spawn */
	UPROPERTY(EditDefaultsOnly, Category = Projectile)
		TSubclassOf<class AProjectile> ProjectileClass;

protected:

	/** Resets HMD orientation in VR. */
	void OnResetVR();

	//sets the players speed
	void SetCharacterSpeed(float Speed);

	//Sets player crouched speed
	void SetCharacterCrouchSpeed(float Speed);

	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);

	//Start Sprinting
	void Sprinting();
	
	//Stop Sprinting
	void SetNormalSpeed();

	//Change Camera
	void ChangeCamera(bool CameraSwitch);

	//Fire the weapon
	void FireWeapon();
		

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
	void TouchStarted(ETouchIndex::Type FingerIndex, FVector Location);

	/** Handler for when a touch input stops. */
	void TouchStopped(ETouchIndex::Type FingerIndex, FVector Location);

	//Called when a key/Button is pressed and collects the item
	UFUNCTION(BlueprintCallable, Category = "Pickups")
	void CollectPickUps();

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// End of APawn interface

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
	//Returns CollectionSphere subobject
	FORCEINLINE class USphereComponent* GetCollectionSphere() const { return CollectionSphere; }
	//Returns WeaponMesh subobject
	FORCEINLINE class USkeletalMeshComponent* GetWeaponMesh() const { return WeaponMesh; }
};

