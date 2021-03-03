// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LoginUI.generated.h"

/**
 * 
 */
class UEditableText;
class UButton;

UCLASS()
class CHATCLIENT_API ULoginUI : public UUserWidget
{
	GENERATED_BODY()


public:
	virtual void	NativeConstruct() override;
	
	UFUNCTION()
	void			OnClickedFunc();

	UEditableText*	_editText;
	UButton*		_loginBt;

};
