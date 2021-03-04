// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */

class UMainUI;
class ULoginUI;
class UMyGameInstance;
class ALoignLevel;

class CHATCLIENT_API UIManager
{
public:
	UIManager();
	~UIManager();
	void ShowToast(FString& text);
	void SetGameInstance();

public:
	UMainUI*		GetMainUI() { return _mainUI;  };
	ULoginUI*		GetLoginUI() { return _loginUI; };
	ALoignLevel*	GetLoginLevel() { return _loginLevel; };
	void			SetMainUI(UMainUI* mainUI) { _mainUI = mainUI; };
	void			SetLoginUI(ULoginUI* loginUI) { _loginUI = loginUI; };
	void			SetLoginLevel(ALoignLevel* loginlevel) { _loginLevel = loginlevel; };

	UMyGameInstance* _gameInstance;
	UMainUI*		_mainUI;
	ULoginUI*		_loginUI;
	ALoignLevel*	_loginLevel;
};
