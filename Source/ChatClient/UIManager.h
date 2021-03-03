// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class UMyGameInstance;
class CHATCLIENT_API UIManager
{
public:
	UIManager();
	~UIManager();
	void ShowToast(FString& text);
	void SetGameInstance();


private:
	UMyGameInstance* _gameInstance;

};
