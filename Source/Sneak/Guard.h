// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Guard.generated.h"

UCLASS()
class SNEAK_API AGuard : public ACharacter
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BoxCollision", meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* BoxCollision;

public:
	// Sets default values for this character's properties
	AGuard();

	FORCEINLINE class UBoxComponent* GetBoxCollsion() const { return BoxCollision; }

	UFUNCTION()
		void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//Set the speed of the guard
	void SetGuardMovementSpeed(float Speed);

	//Speed of the guard in there Idle State
	void SetGuardSpeedWhilstIdle();

	//When the AI has been knocked out
	UFUNCTION(BlueprintCallable)
	void KnockedOut();

	//When the AI has been knocked out
	UFUNCTION(BlueprintCallable)
		void UnKnockedOut();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool isknockedout;

	

	UFUNCTION(BlueprintCallable)
	void PlayerSpotted();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpottedSound")
		class USoundBase* PlayerSpottedSound;
	
};
