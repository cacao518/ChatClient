// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameInstance.h"
#include "../ETC/TcpSocket.h"
#include "Blueprint/UserWidget.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/TextBlock.h"
#include "Blueprint/WidgetTree.h"
#include "Engine/World.h"

UMyGameInstance* UMyGameInstance::_instance {};

UMyGameInstance::UMyGameInstance() :_roomMgr(), _uiMgr()
{
	
}

UMyGameInstance::~UMyGameInstance()
{
	//delete _socket->_userInfo;
	//delete _socket;
}

void UMyGameInstance::OnStart()
{
	Super::OnStart();
	_instance = this;

	/*_socket = Cast<UTcpSocket>(NewObject<UTcpSocket>());
	_instance = this;
	_socket->_userInfo = new FUserInfo();*/
	//_uiMgr.SetGameInstance();
}

UTcpSocket* UMyGameInstance::GetSocket()
{
	return _socket;
}

UMyGameInstance * UMyGameInstance::GetMyGameInstance()
{

	return _instance;
}

RoomManager & UMyGameInstance::GetRoomManager()
{
	return _roomMgr;
}

UIManager & UMyGameInstance::GetUIManager()
{
	return _uiMgr;
}

void UMyGameInstance::JoinRoom(UPARAM(ref) int id)
{
	// 방 참가
	FString sendData = L"/join " + FString::FromInt(id);
	if (_socket->GetisConnect() == true)
		_socket->Send(sendData);
}

void UMyGameInstance::AddRoom(UPARAM(ref) FString name)
{
	// 방 생성
	FString sendData = L"/make " + name;
	if (_socket->GetisConnect() == true)
		_socket->Send(sendData);
}

void UMyGameInstance::ShowWhisperUI(UPARAM(ref) FString name)
{
	FString path = FString("/Game/2DSideScrollerCPP/Blueprints/BP_WhisperUI.BP_WhisperUI_C");
	UClass* whisperUI = ConstructorHelpersInternal::FindOrLoadClass(path, UUserWidget::StaticClass());
	UUserWidget* whisperUIWidget = CreateWidget<UUserWidget>(GetWorld(), whisperUI);

	if (whisperUIWidget != nullptr)
	{
		whisperUIWidget->AddToViewport();
		UTextBlock* nameText = Cast<UTextBlock>(whisperUIWidget->WidgetTree->FindWidget("nameText"));
		nameText->SetText(FText::AsCultureInvariant(name));
	}
}

void UMyGameInstance::Whisper(UPARAM(ref)FString name, UPARAM(ref)FString sendData)
{
	// 귓말날리기
	FString data = L"/w " + name + L" " + sendData;
	if (_socket->GetisConnect() == true)
		_socket->Send(data);

	UMyGameInstance* gameInstance = UMyGameInstance::GetMyGameInstance();
	if (gameInstance == nullptr) return;

	FString text = TEXT("귓속말을 보냈습니다.");
	gameInstance->ShowToast(text);
}

void UMyGameInstance::ShowToast(FString & text)
{
	FString path = FString("/Game/2DSideScrollerCPP/Blueprints/BP_Toast.BP_Toast_C");
	UClass* toastUI = ConstructorHelpersInternal::FindOrLoadClass(path, UUserWidget::StaticClass());
	UUserWidget* toastWidget = CreateWidget<UUserWidget>(GetWorld(), toastUI);

	if (toastWidget != nullptr)
	{
		toastWidget->AddToViewport();
		UTextBlock* _toastText = Cast<UTextBlock>(toastWidget->WidgetTree->FindWidget("ToastText"));
		_toastText->SetText(FText::AsCultureInvariant(text));
		
	}
}

