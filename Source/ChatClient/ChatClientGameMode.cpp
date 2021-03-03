// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "ChatClientGameMode.h"
#include "ChatClientCharacter.h"

AChatClientGameMode::AChatClientGameMode()
{
	// Set default pawn class to our character
	DefaultPawnClass = AChatClientCharacter::StaticClass();	
}
