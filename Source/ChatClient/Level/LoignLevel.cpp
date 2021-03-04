// Fill out your copyright notice in the Description page of Project Settings.

#include "../Level/LoignLevel.h"
#include "../UI/LoginUI.h"
#include "../UI/MainUI.h"
#include "Engine/World.h"
#include "../Manager/MyGameInstance.h"
#include "Blueprint/UserWidget.h"
#include "../ETC/TcpSocket.h"

ALoignLevel::ALoignLevel()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ALoignLevel::BeginPlay()
{
	Super::BeginPlay();

	_gameInstance = Cast<UMyGameInstance>(GetGameInstance());
	if (_gameInstance == nullptr) return;
	if (this == nullptr) return;
	_gameInstance->GetUIManager().SetLoginLevel(this);

	// 소켓 생성
	_gameInstance->_socket = Cast<UTcpSocket>(NewObject<UTcpSocket>());
	if (_gameInstance->_socket != nullptr)
	{
		_gameInstance->_socket->_userInfo._roomId = 1;
		_gameInstance->_socket->_userInfo._roomName = "Lobby";
	}

	// 로그인 UI 생성
	if (loginUI_class != nullptr)
	{
		loginUI = CreateWidget<ULoginUI>(GetWorld(), loginUI_class);
		if (loginUI != nullptr)
		{
			_gameInstance->GetUIManager().SetLoginUI(loginUI);
			loginUI->AddToViewport();
		}
	}

	// 마우스 커서 보이기
	GetWorld()->GetFirstPlayerController()->bShowMouseCursor = true;
	
	// 서버 연결 시도
	if(_gameInstance->GetSocket() != nullptr)
		_gameInstance->GetSocket()->ConnectToServer();
	

}

void ALoignLevel::Tick(float DeltaSeconds)
{
	if (_gameInstance == nullptr) return;
	if (_gameInstance->GetSocket() == nullptr) return;

	FPacket packet = FPacket{ EPacketKind::End, string("")};
	if (_gameInstance->GetSocket()->GetisConnect() == true)
		packet = _gameInstance->GetSocket()->Recv();
	else
		return;

	if (packet.pk == EPacketKind::End) return;

}

void ALoignLevel::CreateMainUI()
{
	if (loginUI == nullptr) return;

	// 로그인창 없애고 채팅창 띄우기
	loginUI->SetVisibility(ESlateVisibility::Collapsed);

	if (mainUI_class != nullptr)
	{
		mainUI = CreateWidget<UMainUI>(GetWorld(), mainUI_class);
		if (mainUI != nullptr)
		{
			_gameInstance->GetUIManager().SetMainUI(mainUI);
			mainUI->AddToViewport();
		}
	}
}

void ALoignLevel::CreateLoginUI()
{
	if (mainUI == nullptr) return;

	// 채팅창 없애고 로그인창 띄우기
	mainUI->SetVisibility(ESlateVisibility::Collapsed);

	if (loginUI_class != nullptr)
	{
		loginUI = CreateWidget<ULoginUI>(GetWorld(), loginUI_class);
		if (loginUI != nullptr)
		{
			_gameInstance->GetUIManager().SetLoginUI(loginUI);
			loginUI->AddToViewport();
		}
	}
}
