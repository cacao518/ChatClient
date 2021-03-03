        // Fill out your copyright notice in the Description page of Project Settings.

#include "TcpSocket.h"

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

	_socket->SetNonBlocking(true);
}

int UTcpSocket::Send(FString& data)
{
	int sentLen = 0;
	
	TCHAR* tchar = L"\r\n\0";
	data.AppendChars(tchar, _tcslen(tchar));
	
	char retBuf[1024];
	wcstombs(retBuf, *data, BUFSIZE);

	_socket->Send(reinterpret_cast<const uint8*>(retBuf), strlen(retBuf), sentLen);
	
	return sentLen;
}

FPacket UTcpSocket::Recv()
{
	int readLen = 0;
	bool isRecv = _socket->Recv(_buf, BUFSIZE, readLen);
	
	// 나중에 앞부분 몇바이트는 패킷종류로 구분해서 읽어내자..
	if (isRecv == true && readLen != 0)
	{
		char ansi[1024];
		memcpy(ansi, _buf, readLen);
		ansi[readLen] = '\0';

		string str(ansi);
		auto offset = str.find(':');
		EPacketKind pakectKind;

		if (offset != string::npos)
		{
			// 패킷넘버가 한자리일경우
			if (offset == ONE_CHAR_PACKET)
			{
				pakectKind = (EPacketKind)stoi(str.erase(0, ONE_CHAR_PACKET));
			}// 패킷넘버가 두자리일경우
			else if(offset == TWO_CHAR_PACKET)
			{
				pakectKind = (EPacketKind)stoi(str.erase(0, TWO_CHAR_PACKET));
			}
			str.erase(0); // ':' 지우기
		}
		
		wchar_t retBuf[1024];
		//mbstowcs(retBuf, ansi, readLen);
		mbstowcs(retBuf, str.c_str(), readLen);
		return FPacket{ pakectKind, FString(retBuf)};
	}

	return FPacket{ EPacketKind::End, FString("none") };
}
