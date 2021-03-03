        // Fill out your copyright notice in the Description page of Project Settings.

#include "TcpSocket.h"
#include "../Manager/MyGameInstance.h"
#include "Engine/World.h"
//#include "Components/ScrollBox.h"
//#include "Components/TextBlock.h"
#include "../UI/MainUI.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/WidgetTree.h"
#include "Runtime/UMG/Public/UMG.h"

UTcpSocket::UTcpSocket()
{
	_userInfo = new FUserInfo();
}
UTcpSocket::~UTcpSocket()
{
	delete _userInfo;
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
	
	// 나중에 앞부분 몇바이트는 패킷종류로 구분해서 읽어내자..
	if (isRecv == true && readLen != 0)
	{
		char ansi[1024];
		memcpy(ansi, _buf, readLen);
		ansi[readLen] = '\0';

		string str(ansi);
	
		EPacketKind pakectKind;
		string data;

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

				wchar_t retBuf[1024];
				mbstowcs(retBuf, data.c_str(), data.size());
				
				PacketProcessor(FPacket{ pakectKind, FString(retBuf) });

				//return FPacket{ pakectKind, FString(retBuf) };

				//wchar_t retBuf[1024];
				////mbstowcs(retBuf, ansi, readLen);
				//mbstowcs(retBuf, str.c_str(), readLen);
				//return FPacket{ pakectKind, FString(retBuf) };

			}
		}
	}

	return FPacket{ EPacketKind::End, FString("none") };
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
	}
}

// 로그인 패킷 받았을 때
void UTcpSocket::GotLogin(FString data)
{
	UMyGameInstance* gameInstance = UMyGameInstance::GetMyGameInstance();
	if (gameInstance == nullptr) return;

	if (gameInstance->GetSocket()->GetisConnect() == false) return;

	// 이름 설정
	_userInfo->name = data;
	FString a = _userInfo->name;

	// 레벨이동
	UGameplayStatics::OpenLevel(gameInstance, FName("Lv_Main"));
}

// 메세지 전송 패킷 받았을 때
void UTcpSocket::GotSendData(const FString & data)
{
	UMyGameInstance* gameInstance = UMyGameInstance::GetMyGameInstance();
	if (gameInstance == nullptr) return;

	string str = TCHAR_TO_ANSI(*data);

	//auto startOffset = str.find('[');
	auto endOffset = str.find(']');

	string name = str.substr(1, endOffset-1); // 이름 얻어내기
	str.erase(0, endOffset+3); // 이름 자르기  " : " 3글자 자르기
	
	FString finalName(name.c_str());
	FString finalData(str.c_str());

	UMainUI* mainUI = gameInstance->GetUIManager().GetMainUI();
	UScrollBox* scrollBox = mainUI->GetScrollBox();

	// 내가 보냈다면 오른쪽으로 말풍선 출력
	if (finalName == _userInfo->name)
	{
		FString path = FString("/Game/2DSideScrollerCPP/Blueprints/BP_BallonRight.BP_BallonRight_C");
		UClass* ballonRightUI = ConstructorHelpersInternal::FindOrLoadClass(path, UUserWidget::StaticClass());
		UUserWidget* ballonRightWidget = CreateWidget<UUserWidget>(gameInstance->GetWorld(), ballonRightUI);

		if (ballonRightWidget != nullptr)
		{
			scrollBox->AddChild(ballonRightWidget);
			scrollBox->ScrollToEnd();
			UTextBlock* _Text = Cast<UTextBlock>(ballonRightWidget->WidgetTree->FindWidget("textBlock"));
			_Text->SetText(FText::AsCultureInvariant(finalData));
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
			scrollBox->AddChild(ballonLeftWidget);
			scrollBox->ScrollToEnd();
			UTextBlock* _Text = Cast<UTextBlock>(ballonLeftWidget->WidgetTree->FindWidget("textBlock"));
			_Text->SetText(FText::AsCultureInvariant(finalData));
		}
	}
	

}
