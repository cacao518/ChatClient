// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/LevelScriptActor.h"
#include "MainLevel.generated.h"

/**
 * 
 */
class UMainUI;
class UMyGameInstance;
UCLASS()
class CHATCLIENT_API AMainLevel : public ALevelScriptActor
{
	GENERATED_BODY()

public:
	AMainLevel();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

public:
	UMyGameInstance* _gameInstance;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Widget")
		TSubclassOf<UMainUI> mainUI_class;
	UMainUI* mainUI;
	
};
