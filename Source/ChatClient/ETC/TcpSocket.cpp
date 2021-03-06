﻿        // Fill out your copyright notice in the Description page of Project Settings.

#include "TcpSocket.h"
#include "../Manager/MyGameInstance.h"
#include "Engine/World.h"
//#include "Components/ScrollBox.h"
//#include "Components/TextBlock.h"
#include "../UI/MainUI.h"
#include "../UI/LoginUI.h"
#include "../Level/LoignLevel.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/WidgetTree.h"
#include "Runtime/UMG/Public/UMG.h"
#include "../Manager/UIManager.h"
#include "../Manager/RoomManager.h"
#include "Components/Button.h"
#include <unordered_set>

UTcpSocket::UTcpSocket()
{
	
}
UTcpSocket::~UTcpSocket()
{
	//UMyGameInstance* gameInstance = UMyGameInstance::GetMyGameInstance();
	//if (gameInstance != nullptr)
	//{
	//	FString text = TEXT("서버와 연결이 끊어졌습니다.");
	//	gameInstance->ShowToast(text);
	//}

	_socket->Close();
}
void UTcpSocket::ConnectToServer()
{
	setlocale(LC_ALL, "Korean");

	_socket = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateSocket(NAME_Stream, TEXT("default"), false);

	FString address = TEXT("127.0.0.1");
	int32 port = 9000;
	FIPv4Address ip;
	FIPv4Address::Parse(address, ip);

	TSharedRef<FInternetAddr> addr = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr();
	addr->SetIp(ip.Value);
	addr->SetPort(port);

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Trying to connect.")));

	_connected = _socket->Connect(*addr);

	if(_connected == true)
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Connect Success.")));

	_socket->SetNonBlocking(true);
}

int UTcpSocket::Send(FString& data)
{
	int sentLen = 0;
	
	TCHAR* tchar = L"\r\n\0";
	data.AppendChars(tchar, _tcslen(tchar));
	
	char retBuf[1024];
	wcstombs(retBuf, *data, 1024);

	_socket->Send(reinterpret_cast<const uint8*>(retBuf), strlen(retBuf), sentLen);
	
	return sentLen;
}

FPacket UTcpSocket::Recv()
{
	int readLen = 0;
	bool isRecv = _socket->Recv(_buf, 1024, readLen);
	
	if (isRecv == true && readLen != 0)
	{
		char ansi[1024];
		memcpy(ansi, _buf, readLen);
		ansi[readLen] = '\0';

		string str(ansi);
	
		EPacketKind pakectKind;
		string data;

		// 스트림 처리
		while (1)
		{
			auto startOffset = str.find('{');

			if (startOffset == string::npos) 
				break;
			else
			{
				// 패킷넘버가 한자리일경우
				if (startOffset == ONE_CHAR_PACKET)
				{
					pakectKind = (EPacketKind)atoi((str.substr(0, ONE_CHAR_PACKET)).c_str());
					str.erase(0, ONE_CHAR_PACKET);
				}// 패킷넘버가 두자리일경우
				else if (startOffset == TWO_CHAR_PACKET)
				{
					pakectKind = (EPacketKind)atoi((str.substr(0, TWO_CHAR_PACKET)).c_str());
					str.erase(0, TWO_CHAR_PACKET);
				}
				str.erase(std::find(str.begin(), str.end(), '{'));


				auto endOffset = str.find('}');
				data = str.substr(0, endOffset);
				str.erase(0, endOffset);
				str.erase(std::find(str.begin(), str.end(), '}'));
				data.push_back('\0');

				//wchar_t retBuf[1024];
				//mbstowcs(retBuf, data.c_str(), data.size());
				
				PacketProcessor(FPacket{ pakectKind, string(data) });

				//return FPacket{ pakectKind, FString(retBuf) };

				//wchar_t retBuf[1024];
				////mbstowcs(retBuf, ansi, readLen);
				//mbstowcs(retBuf, str.c_str(), readLen);
				//return FPacket{ pakectKind, FString(retBuf) };
			}
		}
	}

	return FPacket{ EPacketKind::End, string("none") };
}

void UTcpSocket::PacketProcessor(const FPacket& packet)
{
	switch (packet.pk)
	{
		case EPacketKind::Login:
			GotLogin(packet.data);
			break;

		case EPacketKind::SendData:
			GotSendData(packet.data);
			break;

		case EPacketKind::ShowRoomInfo:
			GotShowRoomInfo(packet.data);
			break;

		case EPacketKind::EnterRoom:
			GotEnterRoom(packet.data);
			break;

		case EPacketKind::LeaveRoom:
			GotLeaveRoom(packet.data);
			break;

		case EPacketKind::ShowRoom:
			GotShowRoom(packet.data);
			break;

		case EPacketKind::JoinRoom:
			GotJoinRoom(packet.data);
			break;

		case EPacketKind::Whisper:
			GotWhisper(packet.data);
			break;
	}
}

// 로그인 패킷 받았을 때
void UTcpSocket::GotLogin(string data)
{
	UMyGameInstance* gameInstance = UMyGameInstance::GetMyGameInstance();
	if (gameInstance == nullptr) return;

	if (gameInstance->GetSocket()->GetisConnect() == false) return;

	// 유니 -> 멀티바이트 변환
	wchar_t retBuf[1024];
	mbstowcs(retBuf, data.c_str(), data.size());

	// 이름 설정
	_userInfo._name = FString(retBuf);

	// 로그인창 없애고 Main UI 생성
	ALoignLevel* loginLevel = gameInstance->GetUIManager().GetLoginLevel();
	if (loginLevel == nullptr) return;

	loginLevel->CreateMainUI();

	//UGameplayStatics::OpenLevel(gameInstance->GetWorld(), FName("Lv_Main"));
}

// 메세지 전송 패킷 받았을 때
void UTcpSocket::GotSendData(const string & data)
{
	UMyGameInstance* gameInstance = UMyGameInstance::GetMyGameInstance();
	if (gameInstance == nullptr) return;

	string str = data;

	//auto startOffset = str.find('[');
	auto endOffset = str.find(']');

	string name = str.substr(1, endOffset-1); // 이름 얻어내기
	str.erase(0, endOffset+3); // 이름 자르기  " : " 3글자 자르기
	name += '\0';

	// 멀티바이트 -> 유니코드로 변환
	wchar_t data_uni[1024];
	mbstowcs(data_uni, str.c_str(), str.size());
	wchar_t name_uni[1024];
	mbstowcs(name_uni, name.c_str(), name.size());

	FString finalName(name_uni);
	FString finalData(data_uni);

	UMainUI* mainUI = gameInstance->GetUIManager().GetMainUI();
	UScrollBox* scrollBox = mainUI->GetScrollBox();

	// 내가 보냈다면 오른쪽으로 말풍선 출력
	if (finalName == _userInfo._name)
	{
		FString path = FString("/Game/2DSideScrollerCPP/Blueprints/BP_BallonRight.BP_BallonRight_C");
		UClass* ballonRightUI = ConstructorHelpersInternal::FindOrLoadClass(path, UUserWidget::StaticClass());
		UUserWidget* ballonRightWidget = CreateWidget<UUserWidget>(gameInstance->GetWorld(), ballonRightUI);

		if (ballonRightWidget != nullptr)
		{
			UTextBlock* _Text = Cast<UTextBlock>(ballonRightWidget->WidgetTree->FindWidget("textBlock"));
			UCanvasPanel* _panel = Cast <UCanvasPanel> (ballonRightWidget->WidgetTree->FindWidget("panel"));

			scrollBox->AddChild(ballonRightWidget);
			scrollBox->ScrollToEnd();
			_Text->SetText(FText::AsCultureInvariant(finalData));

			//UCanvasPanelSlot* Slot = Cast<UCanvasPanelSlot>(_panel->Slot);
			//Slot->SetSize(GetSizeBallon(finalData));
		}
	}
	// 상대방이 보냈다면 왼쪽으로 말풍선 출력
	else
	{
		FString path = FString("/Game/2DSideScrollerCPP/Blueprints/BP_BallonLeft.BP_BallonLeft_C");
		UClass* ballonLeftUI = ConstructorHelpersInternal::FindOrLoadClass(path, UUserWidget::StaticClass());
		UUserWidget* ballonLeftWidget = CreateWidget<UUserWidget>(gameInstance->GetWorld(), ballonLeftUI);

		if (ballonLeftWidget != nullptr)
		{
			UTextBlock* _Text = Cast<UTextBlock>(ballonLeftWidget->WidgetTree->FindWidget("textBlock"));
			UTextBlock* _nameText = Cast<UTextBlock>(ballonLeftWidget->WidgetTree->FindWidget("nameText"));
			UCanvasPanel* _panel = Cast<UCanvasPanel>(ballonLeftWidget->WidgetTree->FindWidget("panel"));
			
			scrollBox->AddChild(ballonLeftWidget);
			scrollBox->ScrollToEnd();
			
			_Text->SetText(FText::AsCultureInvariant(finalData));
			_nameText->SetText(FText::AsCultureInvariant(finalName));

			//UCanvasPanelSlot* Slot = Cast<UCanvasPanelSlot>(_panel->Slot);
			//Slot->SetSize(GetSizeBallon(finalData));
		}
	}
}

void UTcpSocket::GotShowRoomInfo(const string & data)
{
	UMyGameInstance* gameInstance = UMyGameInstance::GetMyGameInstance();
	if (gameInstance == nullptr) return;
	
	// 현재 방 유저 정보 갱신
	gameInstance->GetRoomManager().UpdateCurUserInfo(data);

	if (gameInstance->GetRoomManager().GetCurUserSet().size() == 0) return;

	/* 유저 스크롤 박스에 유저 정보 추가 */
	FString path = FString("/Game/2DSideScrollerCPP/Blueprints/BP_UserBalloon.BP_UserBalloon_C");
	UScrollBox* scrollBox = gameInstance->GetUIManager().GetMainUI()->GetUserScrollBox();					// 유저 정보 담는 스크롤 박스
	if (scrollBox == nullptr) return;
	scrollBox->ClearChildren();

	set<FUserInfo*> userInfoSet = gameInstance->GetRoomManager().GetCurUserSet();
	for (FUserInfo* userInfo : userInfoSet)
	{
		if (userInfo == nullptr) continue;
		if (userInfo->_name == _userInfo._name) // 내이름이랑 같다면?? 
		{
			// UI 반영
			_userInfo._roomName = userInfo->_roomName;
			FString curNum = FString::FromInt(gameInstance->GetRoomManager().GetCurUserSet().size());
			gameInstance->GetUIManager().GetMainUI()->_curUserNumText->SetText(FText::AsCultureInvariant(curNum));			// 현재 방 유저 수
			gameInstance->GetUIManager().GetMainUI()->_curRoomNameText->SetText(FText::AsCultureInvariant(_userInfo._roomName)); // 현재 방 이름
			gameInstance->GetUIManager().GetMainUI()->_myUserNameText->SetText(FText::AsCultureInvariant(_userInfo._name)); // 내 유저 이름
		}

		UClass* userBalloonUI = ConstructorHelpersInternal::FindOrLoadClass(path, UUserWidget::StaticClass());
		UUserWidget* userBalloonWidget = CreateWidget<UUserWidget>(gameInstance->GetWorld(), userBalloonUI);  // 유저 정보 풍선
		UTextBlock* _nameText = Cast<UTextBlock>(userBalloonWidget->WidgetTree->FindWidget("nameText"));     // 유저 정보 풍선의 유저이름
		_nameText->SetText(FText::AsCultureInvariant(userInfo->_name));									// 유저 정보 풍선의 유저이름 셋팅
		scrollBox->AddChild(userBalloonWidget);													//스크롤 박스에 추가
	}

	// 추후에 내 유저 풍선은 제일 위로 올라가게 만들기

}

void UTcpSocket::GotShowRoom(const string & data)
{
	UMyGameInstance* gameInstance = UMyGameInstance::GetMyGameInstance();
	if (gameInstance == nullptr) return;

	// 전체 방 유저 정보 갱신
	gameInstance->GetRoomManager().UpdateAllRoomInfo(data);
	unordered_map<int, FRoomInfo> id_roomMap = gameInstance->GetRoomManager().GetID_RoomMap();

	if (id_roomMap.size() == 0) return;

	//// 내 방 이름 갱신
	//_userInfo._roomName = id_roomMap[_userInfo._roomId]._name;
	//int curNum = id_roomMap[_userInfo._roomId]._userNum;
	//gameInstance->GetUIManager().GetMainUI()->_curUserNumText->SetText(FText::AsCultureInvariant(FString::FromInt(curNum)));			// 현재 방 유저 수
	//gameInstance->GetUIManager().GetMainUI()->_curRoomNameText->SetText(FText::AsCultureInvariant(_userInfo._roomName)); // 현재 방 이름

	/* 방 스크롤 박스에 방 정보 추가 */
	FString path = FString("/Game/2DSideScrollerCPP/Blueprints/BP_RoomSlot.BP_RoomSlot_C");
	UUserWidget* roomListUIWidget = gameInstance->GetUIManager().GetMainUI()->_roomListUIWidget;
	UScrollBox* scrollBox = Cast<UScrollBox>(roomListUIWidget->WidgetTree->FindWidget("roomScroll"));

	if (scrollBox == nullptr) return;
	scrollBox->ClearChildren();

	for (auto iter : id_roomMap)
	{
		FRoomInfo roomInfo = iter.second;

		UClass* roomSlotUI = ConstructorHelpersInternal::FindOrLoadClass(path, UUserWidget::StaticClass());
		UUserWidget* roomSlotUIWidget = CreateWidget<UUserWidget>(gameInstance->GetWorld(), roomSlotUI);  // 방 정보 풍선
		UTextBlock* _roomNumText = Cast<UTextBlock>(roomSlotUIWidget->WidgetTree->FindWidget("roomNumText"));     // 방 정보 풍선의 방번호
		UTextBlock* _nameText = Cast<UTextBlock>(roomSlotUIWidget->WidgetTree->FindWidget("roomNameText"));     // 방 정보 풍선의 방이름
		UTextBlock* _userNumText = Cast<UTextBlock>(roomSlotUIWidget->WidgetTree->FindWidget("userNumText"));     // 방 정보 풍선의 유저수
		UButton* _enterBt = Cast<UButton>(roomSlotUIWidget->WidgetTree->FindWidget("enterBt"));					// 방 정보 풍선의 입장버튼

		_roomNumText->SetText(FText::AsCultureInvariant(FString::FromInt(roomInfo._id)));
		_nameText->SetText(FText::AsCultureInvariant(roomInfo._name));
		_userNumText->SetText(FText::AsCultureInvariant(FString::FromInt(roomInfo._userNum)));
		_nameText->SetText(FText::AsCultureInvariant(roomInfo._name));
		scrollBox->AddChild(roomSlotUIWidget);													//스크롤 박스에 추가

		//_enterBt->OnClicked.AddDynamic(this, &UTcpSocket::ButtonClickEvent);
	}

}

void UTcpSocket::GotEnterRoom(const string & data)
{
	UMyGameInstance* gameInstance = UMyGameInstance::GetMyGameInstance();
	if (gameInstance == nullptr) return;

	// 현재 방 유저 정보 갱신하기
	FString sendData = L"/r " + FString::FromInt(gameInstance->GetSocket()->_userInfo._roomId);
	if (gameInstance->GetSocket()->GetisConnect() == true)
		gameInstance->GetSocket()->Send(sendData);
}

void UTcpSocket::GotLeaveRoom(const string & data)
{
	UMyGameInstance* gameInstance = UMyGameInstance::GetMyGameInstance();
	if (gameInstance == nullptr) return;

	// 현재 방 유저 정보 갱신하기
	FString sendData = L"/r " + FString::FromInt(gameInstance->GetSocket()->_userInfo._roomId);
	if (gameInstance->GetSocket()->GetisConnect() == true)
		gameInstance->GetSocket()->Send(sendData);
}

void UTcpSocket::GotJoinRoom(const string & data)
{
	UMyGameInstance* gameInstance = UMyGameInstance::GetMyGameInstance();
	if (gameInstance == nullptr) return;

	_userInfo._roomId = atoi(data.c_str()); // 현재 내 방 번호 갱신

	// 새로 들어간방 방 유저 정보 갱신하기
	FString sendData = L"/r " + FString::FromInt(_userInfo._roomId);
	if (gameInstance->GetSocket()->GetisConnect() == true)
		gameInstance->GetSocket()->Send(sendData);

	// 채팅창 비우기
	UMainUI* mainUI = gameInstance->GetUIManager().GetMainUI();
	UScrollBox* chatScrollBox = mainUI->GetScrollBox();
	chatScrollBox->ClearChildren();

	if (mainUI->_roomListUIWidget != nullptr)
		mainUI->_roomListUIWidget->RemoveFromViewport();

	FString text = TEXT("방에 참가하였습니다.");
	gameInstance->ShowToast(text);
}

void UTcpSocket::GotWhisper(const string & data)
{
	UMyGameInstance* gameInstance = UMyGameInstance::GetMyGameInstance();
	if (gameInstance == nullptr) return;

	// 멀티바이트 -> 유니코드로 변환
	wchar_t data_uni[1024];
	mbstowcs(data_uni, data.c_str(), data.size());

	FString text = FString(data_uni);
	gameInstance->ShowToast(text);
}

FVector2D UTcpSocket::GetSizeBallon(FString data)
{
	float x = -300.f, y = 120.f;
	float maxX = 0; // 현재까지 가장 긴 x길이

	for (int i = 0; i < data.Len(); i++)
	{
		if (data[i] == '\n') y += 40;					// 줄바꿈

		if (data[i] >= 32 && data[i] <= 126)        // 영어,숫자,기본특수문자  
		{
			if (maxX <= -500)
				y += 40;
			else if (maxX <= x)
			{
				maxX -= 15;
				x = 0;
			}
			else
				x += 15;
		}

		if (data[i] & 0x80)				// 한글체크 
		{
			if (maxX <= -500)
				y += 40;
			else if (maxX <= x)
			{
				maxX -= 30;
				x = 0;
			}
			else
				x -= 30;
		}
	}
	return FVector2D(maxX, y);
}

