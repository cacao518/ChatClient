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

