// Copyright Epic Games, Inc. All Rights Reserved.

#include "FPQuickStartGameMode.h"
#include "FPQuickStartHUD.h"
#include "FPQuickStartCharacter.h"
#include "UObject/ConstructorHelpers.h"

AFPQuickStartGameMode::AFPQuickStartGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPersonCPP/Blueprints/FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = AFPQuickStartHUD::StaticClass();
}
