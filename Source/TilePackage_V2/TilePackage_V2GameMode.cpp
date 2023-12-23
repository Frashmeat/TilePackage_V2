// Copyright Epic Games, Inc. All Rights Reserved.

#include "TilePackage_V2GameMode.h"
#include "TilePackage_V2Character.h"
#include "UObject/ConstructorHelpers.h"

ATilePackage_V2GameMode::ATilePackage_V2GameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
