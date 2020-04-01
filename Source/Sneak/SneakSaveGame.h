// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "SneakSaveGame.generated.h"

/**
 * 
 */
UCLASS()
class SNEAK_API USneakSaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	//Value that the highscore will be saved too.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int HighscoreSeconds;

	//SaveSlot
	UPROPERTY(VisibleAnywhere, Category = Basic)
		FString SaveSlotName;

	UPROPERTY(VisibleAnywhere, Category = Basic)
		uint32 UserIndex;
	
	USneakSaveGame();
};
