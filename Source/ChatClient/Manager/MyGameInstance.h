// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "../ETC/TcpSocket.h"
#include "RoomManager.h"
#include "UIManager.h"

#include "MyGameInstance.generated.h"
/**
 * 
 */

class UTcpSocket;
class RoomManager;
class UIManager;
class FText;
UCLASS()
class CHATCLIENT_API UMyGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UMyGameInstance();
	~UMyGameInstance();

	virtual void OnStart() override;

	static UMyGameInstance* GetMyGameInstance();
	UTcpSocket*		GetSocket();
	RoomManager&	GetRoomManager();
	UIManager&		GetUIManager();
	void			ShowToast(FString& text);

public:
	UTcpSocket* _socket;
	static UMyGameInstance* _instance;
	RoomManager				_roomMgr;
	UIManager				_uiMgr;
};
