// Fill out your copyright notice in the Description page of Project Settings.

#include "RedButton.h"
#include "SneakCharacter.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ARedButton::ARedButton()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ButtonMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ButtonMesh"));
	ButtonMesh->SetupAttachment(RootComponent);

	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	BoxCollision->SetBoxExtent(FVector(75.f,75.f,50.f), true);
	BoxCollision->OnComponentBeginOverlap.AddDynamic(this, &ARedButton::OnOverlapBegin);
	BoxCollision->SetupAttachment(RootComponent);

	CanPress = false;

}

// Called when the game starts or when spawned
void ARedButton::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ARedButton::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void ARedButton::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{	
	ASneakCharacter* Player = Cast<ASneakCharacter>(UGameplayStatics::GetPlayerPawn(this, 0));
	if (OtherActor && (OtherActor != this) && OtherComp)
	{
		if (OtherActor == Player)
		{
			CanPress = true;
		
		}
	}
}

void ARedButton::OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{

}
