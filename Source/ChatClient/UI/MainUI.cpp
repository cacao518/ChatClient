// Fill out your copyright notice in the Description page of Project Settings.


#include "MainUI.h"
#include "Components/Button.h"
#include "Components/EditableText.h"
#include "Blueprint/WidgetTree.h"
#include "Engine/World.h"
#include "../Manager/MyGameInstance.h"
#include "Components/ScrollBox.h"

void UMainUI::NativeConstruct()
{
	UMyGameInstance* gameInstance = Cast< UMyGameInstance>(GetGameInstance());
	if (gameInstance == nullptr) return;

	//UE_LOG(LogTemp, Warning, TEXT("Test"));
	_sendBt = Cast<UButton>(WidgetTree->FindWidget("SendBt"));
	_editText = Cast<UEditableText>(WidgetTree->FindWidget("editText"));
	_scrollBox = Cast<UScrollBox>(WidgetTree->FindWidget("scrollBox"));
	_sendBt->OnClicked.AddDynamic(this, &UMainUI::OnClickedFunc);

	FString text = TEXT("로그인 되었습니다.");
	gameInstance->ShowToast(text);

}

void UMainUI::OnClickedFunc()
{
	UMyGameInstance* gameInstance = Cast< UMyGameInstance>(GetGameInstance());

	if (gameInstance == nullptr) return;
	if (gameInstance->GetSocket()->GetisConnect() == false) return;

	// 채팅 보내기
	FString editTextStr = _editText->GetText().ToString();
	if (gameInstance->GetSocket()->GetisConnect() == true)
		gameInstance->GetSocket()->Send( editTextStr );

	_editText->SetText(FText::FromString(""));

}
