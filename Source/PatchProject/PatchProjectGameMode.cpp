// Copyright Epic Games, Inc. All Rights Reserved.

#include "PatchProjectGameMode.h"
#include "PatchProjectPlayerController.h"
#include "PatchProjectCharacter.h"
#include "UObject/ConstructorHelpers.h"

APatchProjectGameMode::APatchProjectGameMode()
{
	// use our custom PlayerController class
	PlayerControllerClass = APatchProjectPlayerController::StaticClass();

	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/Characters/Mannequin/Blueprints/TopDownCharacter"));
	if (PlayerPawnBPClass.Class != nullptr)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}