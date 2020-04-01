// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "SneakGameMode.generated.h"

UENUM(BlueprintType)
enum class EPlayState : uint8
{
	EPlaying,
	EGameOver,
	EWon,
	EUnknown
};

UCLASS(minimalapi)
class ASneakGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ASneakGameMode();
	
	//Returns the current playing state
	UFUNCTION(BlueprintCallable)
	EPlayState GetCurrentState() const;
	//Set new current state
	UFUNCTION(BlueprintCallable)
	void SetCurrentState(EPlayState NewState);
	//Called when player is detected - setting game to gameover.
	UFUNCTION(BlueprintCallable)
	void SetGameOver();
	//Called when player reaches the end with collectible
	UFUNCTION(BlueprintCallable)
	void SetWon();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool MoveAI;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool StopTimer;

	UPROPERTY(VisibleAnywhere,BlueprintReadWrite)
	int Seconds;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	int Highscore;


protected:
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "HUD", Meta = (BlueprintProtected = "true"))
	TSubclassOf<class UUserWidget> HUDWidgetClass;



	UPROPERTY()
	class UUserWidget* CurrentWidget;

	void StopMovement();

	

private:

	//Keeps track of the current playing state
	EPlayState CurrentState;

	//Handle any function calls that rely upon changing the playing state of our game
	void HandleNewState(EPlayState NewState);

};



