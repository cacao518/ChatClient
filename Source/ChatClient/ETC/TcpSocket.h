// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Engine.h"
#include "Runtime/Networking/Public/Networking.h"
#include "Sockets.h"
#include "SocketSubsystem.h"

#include "CoreMinimal.h"
#include <string>
#include <set>
#include "UObject/Object.h"
//#include "Kismet/BlueprintFunctionLibrary.h"
#include "TcpSocket.generated.h"

using namespace std;

/**
 * 
 */

constexpr char ONE_CHAR_PACKET = 1;
constexpr char TWO_CHAR_PACKET = 2;

UENUM(BlueprintType)
enum class EPacketKind : uint8 {
	Login = 0,			//		로그인 : /login 닉네임
	Help,				//		명령어 안내 : /help
	Exit,				//		종료 : /exit
	UnrealCheck,		//		언리얼 클라 체크
	ShowUser,			//		전체 유저 출력 : /user
	ShowRoom,			//		전체 방 출력 : /room
	ShowRoomInfo,		//		선택 방 정보 출력 : /r 방번호
	ShowUserInfo,		//		선택 유저 정보 출력 : /i 이름
	Whisper,			//		귓속말 : /w 이름 
	MakeRoom,			//		방만들기 : /make 최대인원 방제목
	JoinRoom,			//		방 참여 : /join 방번호  
	Kick,				//		강퇴(방장만가능) : /kick 이름
	SendData,
	EnterRoom,
	End
};

struct FUserInfo {
	int					_id;			// 유저 아이디
	FString				_name;			// 유저 이름
	int					_roomId;		// 유저가 있는 방 정보
	FString				_roomName;		// 유저가 있는 방 정보
};
struct FRoomInfo {
	int					_id;				// 방아이디
	FString				_name;			// 방이름
	bool				_isLobby;		// 로비인가?
	FUserInfo			_master;			// 방장 정보
	set<FUserInfo>		_userInfoSet;	// 방에 있는 유저 정보
};


USTRUCT(Atomic, BlueprintType)
struct FPacket {

	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EPacketKind pk;

	string data;
};

UCLASS(Blueprintable, BlueprintType)
class CHATCLIENT_API UTcpSocket : public UObject
{
	GENERATED_BODY()

	using ushort = unsigned short;
	using uint32 = unsigned int;

public:
	UTcpSocket();
	~UTcpSocket();

	UFUNCTION(BlueprintCallable)
	void ConnectToServer(); 

	UFUNCTION(BlueprintCallable)
	int Send(FString& data);

	UFUNCTION(BlueprintCallable)
	FPacket Recv();

	UFUNCTION(BlueprintCallable)
	bool GetisConnect() { return _connected; };

	void PacketProcessor(const FPacket& packet);

	void GotLogin(string data);
	void GotSendData(const string& data);
	void GotShowRoomInfo(const string& data);
	void GotEnterRoom(const string& data);

	FVector2D GetSizeBallon(FString data);

public:
	const int BUFSIZE = 1024;
	ushort	_writeOffset;
	uint8	_buf[1024];

	FSocket* _socket;
	bool _connected = false;

	FUserInfo _userInfo;			// 유저 정보

};
