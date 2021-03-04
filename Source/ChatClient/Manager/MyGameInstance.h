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
UCLASS(Blueprintable, BlueprintType)
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


	UFUNCTION(BlueprintCallable)
	void	JoinRoom(UPARAM(ref) int id);
	UFUNCTION(BlueprintCallable)
	void	AddRoom(UPARAM(ref) FString name);
	UFUNCTION(BlueprintCallable)
	void	ShowWhisperUI(UPARAM(ref) FString name);
	UFUNCTION(BlueprintCallable)
	void	Whisper(UPARAM(ref) FString name, UPARAM(ref)FString sendData);

	void	ShowToast(FString& text);

public:
	UTcpSocket* _socket;
	static UMyGameInstance* _instance;
	RoomManager				_roomMgr;
	UIManager				_uiMgr;
};
