// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "SneakGameMode.h"
#include "SneakCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "SneakSaveGame.h"

ASneakGameMode::ASneakGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

	

	Seconds = 0;
	

	MoveAI = false;
	StopTimer = false;
}

void ASneakGameMode::BeginPlay()
{
	Super::BeginPlay();

	if (HUDWidgetClass != nullptr)
	{
		CurrentWidget = CreateWidget<UUserWidget>(GetWorld(), HUDWidgetClass);
		if (CurrentWidget != nullptr)
		{
			CurrentWidget->AddToViewport();
		}

	}

	SetCurrentState(EPlayState::EPlaying);
}


void ASneakGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}



void ASneakGameMode::SetGameOver()
{
	SetCurrentState(EPlayState::EGameOver);
}

void ASneakGameMode::SetWon()
{
	SetCurrentState(EPlayState::EWon);
}

void ASneakGameMode::StopMovement()
{
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
	if (PlayerController)
	{
		PlayerController->SetCinematicMode(true, true, true);
	}

	PlayerController->bShowMouseCursor = true;
	PlayerController->bEnableClickEvents = true;
	PlayerController->bEnableMouseOverEvents = true;
}

EPlayState ASneakGameMode::GetCurrentState() const
{
	return CurrentState;
}

void ASneakGameMode::SetCurrentState(EPlayState NewState)
{
	CurrentState = NewState;

	HandleNewState(CurrentState);
}

void ASneakGameMode::HandleNewState(EPlayState NewState)
{
	switch (NewState)
	{
	case EPlayState::EPlaying :
	{
		UE_LOG(LogTemp, Warning, TEXT("Playing"));

		
	}
	break;
	case EPlayState::EWon :
	{
		UE_LOG(LogTemp, Warning, TEXT("Won"));
		StopMovement();
		StopTimer = true;
	}
	break;
	case EPlayState::EGameOver :
	{
		UE_LOG(LogTemp, Warning, TEXT("Gameover"));
		StopMovement();
		StopTimer = true;
	}
	break;
	case EPlayState::EUnknown:
	{

	}
	break;
	}

}

