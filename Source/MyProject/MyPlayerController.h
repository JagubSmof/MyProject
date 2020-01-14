// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MyProjectPawn.h"
#include "MyPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class MYPROJECT_API AMyPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AMyPlayerController();
	//bool facingCursor;
	AMyProjectPawn* MyPawn;

protected:
	void SetNewShootLocation(FVector);
	void MoveToMouseCursor();
	void FireWeapon();
	//void OnSetDestinationPressed();
	//void OnSetDestinationReleased();
	virtual void SetupInputComponent() override;
	virtual void PlayerTick(float DeltaTime) override;
};
