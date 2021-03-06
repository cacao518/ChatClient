// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainUI.generated.h"

/**
 * 
 */
class UEditableText;
class UButton;
class UScrollBox;
class UTextBlock;
UCLASS()
class CHATCLIENT_API UMainUI : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void	NativeConstruct() override;

	UFUNCTION()
		void			OnClickedFunc_SendBt();
	UFUNCTION()
		void			OnClickedFunc_RoomAddBt();
	UFUNCTION()
		void			OnClickedFunc_RoomListBt();
	UFUNCTION()
		void			OnClickedFunc_ExitBt();

public:
	UScrollBox*		GetScrollBox() { return _scrollBox; };
	UScrollBox*		GetUserScrollBox() { return _userScroll; };

public:
	UEditableText*	_editText;
	UScrollBox*		_scrollBox;

	UScrollBox*		_userScroll;
	UTextBlock*		_myUserNameText;
	UTextBlock*		_curRoomNameText;
	UTextBlock*		_curUserNumText;

	UButton*		_sendBt;
	UButton*		_roomAddBt;
	UButton*		_roomListBt;
	UButton*		_exitBt;

	UUserWidget*	_roomListUIWidget = nullptr;
	UUserWidget*	_roomAddUIWidget = nullptr;
};
