// Fill out your copyright notice in the Description page of Project Settings.


#include "MainUI.h"
//#include "Components/Button.h"
#include "Components/EditableText.h"
#include "Blueprint/WidgetTree.h"
#include "Engine/World.h"
#include "../Manager/MyGameInstance.h"
//#include "Components/ScrollBox.h"
#include "Runtime/UMG/Public/UMG.h"

void UMainUI::NativeConstruct()
{
	UMyGameInstance* gameInstance = Cast< UMyGameInstance>(GetGameInstance());
	if (gameInstance == nullptr) return;

	//UE_LOG(LogTemp, Warning, TEXT("Test"));
	_scrollBox = Cast<UScrollBox>(WidgetTree->FindWidget("scroll"));
	_sendBt = Cast<UButton>(WidgetTree->FindWidget("SendBt"));
	_editText = Cast<UEditableText>(WidgetTree->FindWidget("editText"));
	_userScroll = Cast<UScrollBox>(WidgetTree->FindWidget("userScroll"));

	_myUserNameText = Cast<UTextBlock>(WidgetTree->FindWidget("nameText"));
	_curRoomNameText = Cast<UTextBlock>(WidgetTree->FindWidget("roomNameText"));
	_curUserNumText = Cast<UTextBlock>(WidgetTree->FindWidget("userNumText"));

	_sendBt->OnClicked.AddDynamic(this, &UMainUI::OnClickedFunc);


	// 현재 방 유저 정보 갱신하기
	FString sendData = L"/r " + FString::FromInt(gameInstance->GetSocket()->_userInfo._roomId);
	if (gameInstance->GetSocket()->GetisConnect() == true)
		gameInstance->GetSocket()->Send(sendData);
}

void UMainUI::OnClickedFunc()
{
	UMyGameInstance* gameInstance = Cast< UMyGameInstance>(GetGameInstance());

	if (gameInstance == nullptr) return;
	if (gameInstance->GetSocket()->GetisConnect() == false) return;
	if (_editText == nullptr) return;
	// 채팅 보내기
	FString editTextStr = _editText->GetText().ToString();
	if (gameInstance->GetSocket()->GetisConnect() == true)
		gameInstance->GetSocket()->Send( editTextStr );

	_editText->SetText(FText::FromString(""));

}
