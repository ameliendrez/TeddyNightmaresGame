// Copyright Epic Games, Inc. All Rights Reserved.

#include "TeddyNightmaresGameMode.h"
#include "TeddyNightmaresCharacter.h"
#include "UObject/ConstructorHelpers.h"

ATeddyNightmaresGameMode::ATeddyNightmaresGameMode()
{
	// set default pawn class to our Blueprinted character
	//static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/Blueprints/MyCharacterExample_BP"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
