// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RedButton.generated.h"

UCLASS()
class SNEAK_API ARedButton : public AActor
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "BoxCollision", meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* BoxCollision;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "ButtonMesh", meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* ButtonMesh;
	
public:	
	// Sets default values for this actor's properties
	ARedButton();

	FORCEINLINE class UBoxComponent* GetBoxCollision() const { return BoxCollision; }

	FORCEINLINE class UStaticMeshComponent* GetButtonMesh() const { return ButtonMesh; }

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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		bool CanPress;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		bool IsPressed;
	
};
