// Fill out your copyright notice in the Description page of Project Settings.

#include "../Level/LoignLevel.h"
#include "../UI/LoginUI.h"
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

	if (loginUI_class != nullptr)
	{
		loginUI = CreateWidget<ULoginUI>(GetWorld(), loginUI_class);
		if (loginUI != nullptr)
		{
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

	FPacket packet = FPacket{ EPacketKind::End, FString("")};
	if (_gameInstance->GetSocket()->GetisConnect() == true)
		packet = _gameInstance->GetSocket()->Recv();
	else
		return;

	if (packet.pk == EPacketKind::End) return;

}
