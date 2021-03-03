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

UCLASS()
class CHATCLIENT_API UMainUI : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void	NativeConstruct() override;

	UFUNCTION()
		void			OnClickedFunc();

public:
	UEditableText*	_editText;
	UButton*		_sendBt;
	UScrollBox*		_scrollBox;
};
