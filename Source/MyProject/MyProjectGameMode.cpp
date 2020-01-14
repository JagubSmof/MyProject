// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "MyProjectGameMode.h"
#include "MyProjectPawn.h"
#include "MyPlayerController.h"

AMyProjectGameMode::AMyProjectGameMode()
{
	// set default pawn class to our character class
	DefaultPawnClass = AMyProjectPawn::StaticClass();
	PlayerControllerClass = AMyPlayerController::StaticClass();
}

