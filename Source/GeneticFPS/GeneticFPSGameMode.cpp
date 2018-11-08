// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "GeneticFPSGameMode.h"
#include "GeneticFPSHUD.h"
#include "GeneticFPSCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "DNA.h"

AGeneticFPSGameMode::AGeneticFPSGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPersonCPP/Blueprints/FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = AGeneticFPSHUD::StaticClass();

}
