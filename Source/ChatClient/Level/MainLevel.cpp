// Fill out your copyright notice in the Description page of Project Settings.


#include "MainLevel.h"
#include "../UI/mainUI.h"
#include "Engine/World.h"
#include "../Manager/MyGameInstance.h"
#include "Blueprint/UserWidget.h"
#include "../ETC/TcpSocket.h"

AMainLevel::AMainLevel()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AMainLevel::BeginPlay()
{
	Super::BeginPlay();

	_gameInstance = Cast<UMyGameInstance>(GetGameInstance());
	if (_gameInstance == nullptr) return;

	if (mainUI_class != nullptr)
	{
		mainUI = CreateWidget<UMainUI>(GetWorld(), mainUI_class);
		if (mainUI != nullptr)
		{
			_gameInstance->GetUIManager().SetMainUI(mainUI);
			mainUI->AddToViewport();
		}
	}

	// 마우스 커서 보이기
	GetWorld()->GetFirstPlayerController()->bShowMouseCursor = true;

}

void AMainLevel::Tick(float DeltaSeconds)
{
	if (_gameInstance == nullptr) return;
	if (_gameInstance->GetSocket() == nullptr) return;

	FPacket packet = FPacket{ EPacketKind::End, FString("") };
	if (_gameInstance->GetSocket()->GetisConnect() == true)
		packet = _gameInstance->GetSocket()->Recv();
	else
		return;

	if (packet.pk == EPacketKind::End) return;
}
