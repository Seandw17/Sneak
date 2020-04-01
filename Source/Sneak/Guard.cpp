// Fill out your copyright notice in the Description page of Project Settings.

#include "Guard.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "SneakCharacter.h"
#include "SneakGameMode.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"

// Sets default values
AGuard::AGuard()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	BoxCollision->SetBoxExtent(FVector(50.f,75.f,50.f), true);
	BoxCollision->OnComponentBeginOverlap.AddDynamic(this, &AGuard::OnOverlapBegin);
	BoxCollision->SetupAttachment(RootComponent);
	

	isknockedout = false;
	
}

// Called when the game starts or when spawned
void AGuard::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGuard::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}

// Called to bind functionality to input
void AGuard::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AGuard::SetGuardMovementSpeed(float Speed)
{
	GetCharacterMovement()->MaxWalkSpeed = Speed;
}

void AGuard::SetGuardSpeedWhilstIdle()
{
	SetGuardMovementSpeed(300.f);
}

void AGuard::KnockedOut()
{
	isknockedout = true;
	
}

void AGuard::UnKnockedOut()
{
	isknockedout = false;
	
}
void AGuard::PlayerSpotted()
{
	if (PlayerSpottedSound != NULL)
	{
		UGameplayStatics::PlaySoundAtLocation(this, PlayerSpottedSound, GetActorLocation());
	}
	
}



void AGuard::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ASneakCharacter* Player = Cast<ASneakCharacter>(UGameplayStatics::GetPlayerPawn(this, 0));
	ASneakGameMode* GameMode = (ASneakGameMode*)GetWorld()->GetAuthGameMode();
	if (OtherActor && (OtherActor != this) && OtherComp)
	{
		if (isknockedout == false)
		{
			if (OtherActor == Player)
			{
				GameMode->SetGameOver();
				UE_LOG(LogTemp, Warning, TEXT("Overlapped"));
			}
		}
		

	}
}

void AGuard::OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{

}