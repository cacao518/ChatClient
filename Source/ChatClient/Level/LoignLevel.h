// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/LevelScriptActor.h"
#include "LoignLevel.generated.h"

/**
 * 
 */
class ULoginUI;
class UMainUI;
class UMyGameInstance;
UCLASS()
class CHATCLIENT_API ALoignLevel : public ALevelScriptActor
{
	GENERATED_BODY()
	
public:
	ALoignLevel();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	void CreateMainUI();

public:
	UMyGameInstance* _gameInstance;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Widget")
		TSubclassOf<ULoginUI> loginUI_class;
	ULoginUI* loginUI;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Widget")
		TSubclassOf<UMainUI> mainUI_class;
	UMainUI* mainUI;

};
