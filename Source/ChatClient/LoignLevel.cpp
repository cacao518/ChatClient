// Fill out your copyright notice in the Description page of Project Settings.

#include "LoignLevel.h"
#include "LoginUI.h"
#include "Engine/World.h"
#include "MyGameInstance.h"
#include "Blueprint/UserWidget.h"
#include "TcpSocket.h"

ALoignLevel::ALoignLevel()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ALoignLevel::BeginPlay()
{
	Super::BeginPlay();

	_gameInstance = (UMyGameInstance*)(GetGameInstance());

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
	UE_LOG(LogTemp, Warning, TEXT("1"));
	FPacket packet;
	if (_gameInstance->GetSocket()->GetisConnect() == true)
		packet = _gameInstance->GetSocket()->Recv();
	
	UE_LOG(LogTemp, Warning, TEXT("2"));


	if (packet.pk == EPacketKind::End) return;



	UE_LOG(LogTemp, Warning, TEXT("3"));
	if (packet.pk == EPacketKind::Login)
	{
		UE_LOG(LogTemp, Warning, TEXT("4"));
		_gameInstance->GetSocket()->_userInfo->name = packet.data;
		UGameplayStatics::OpenLevel(this, FName("Lv_Main"));
		UE_LOG(LogTemp, Warning, TEXT("5"));
		FString text = TEXT("로그인 되었습니다.");
		_gameInstance->ShowToast(text);
	}
}
