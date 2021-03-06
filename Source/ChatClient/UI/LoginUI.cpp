﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "../UI/LoginUI.h"
#include "Components/Button.h"
#include "Components/EditableText.h"
#include "Blueprint/WidgetTree.h"
#include "Engine/World.h"
#include "../Manager/MyGameInstance.h"
#include "../Manager/UIManager.h"

void ULoginUI::NativeConstruct()
{
	UE_LOG(LogTemp, Warning, TEXT("Test"));
	_loginBt = Cast<UButton>(WidgetTree->FindWidget("LoginBt"));
	_editText = Cast<UEditableText>(WidgetTree->FindWidget("EditText"));
	_loginBt->OnClicked.AddDynamic(this, &ULoginUI::OnClickedFunc);


}

void ULoginUI::OnClickedFunc()
{
	UE_LOG(LogTemp, Warning, TEXT("Test"));
	UMyGameInstance* gameInstance = Cast< UMyGameInstance>(GetGameInstance());
	if (gameInstance == nullptr) return;

	// 서버 연결 시도
	//if (gameInstance->GetSocket() != nullptr &&
	//	gameInstance->GetSocket()->GetisConnect() == false)
	//	gameInstance->GetSocket()->ConnectToServer();

	if (gameInstance->GetSocket()->GetisConnect() == false)
	{
		UE_LOG(LogTemp, Warning, TEXT("서버와 연결이 끊어졌습니다."));
		FString text = TEXT("서버와 연결이 끊어졌습니다.");
		gameInstance->ShowToast(text);
		return;
	}

	FString editTextStr = _editText->GetText().ToString();

	if (editTextStr.Len() == 0)
	{
		FString text = TEXT("아이디를 입력하세요.");
		gameInstance->ShowToast(text);
		return;
	}
	if (editTextStr.Find(" ") != -1)
	{
		FString text = TEXT("공백을 넣을 수 없습니다.");
		gameInstance->ShowToast(text);
		return;
	}
	if (editTextStr.Len() < 2)
	{
		FString text = TEXT("2글자 이상 입력해주세요.");
		gameInstance->ShowToast(text);
		return;
	}
	if (editTextStr.Len() >= 7)
	{
		FString text = TEXT("6글자까지 가능합니다.");
		gameInstance->ShowToast(text);
		return;
	}

	if (gameInstance->GetSocket() == nullptr) return;

	// 로그인 체크
	if (gameInstance->GetSocket()->GetisConnect() == false)
	{
		gameInstance->GetSocket()->ConnectToServer();
		return;
	}

	// 언리얼 클라 확인 패킷 보내기
	FString sendData = L"/i am unreal";
	if (gameInstance->GetSocket()->GetisConnect() == true)
		gameInstance->GetSocket()->Send(sendData);

	// 로그인 시도
	sendData = L"/login " + editTextStr;
	if (gameInstance->GetSocket()->GetisConnect() == true)
		gameInstance->GetSocket()->Send(sendData);
}
