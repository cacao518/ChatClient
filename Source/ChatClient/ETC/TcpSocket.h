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
	Login = 0,			//		�α��� : /login �г���
	Help,				//		��ɾ� �ȳ� : /help
	Exit,				//		���� : /exit
	UnrealCheck,		//		�𸮾� Ŭ�� üũ
	ShowUser,			//		��ü ���� ��� : /user
	ShowRoom,			//		��ü �� ��� : /room
	ShowRoomInfo,		//		���� �� ���� ��� : /r ���ȣ
	ShowUserInfo,		//		���� ���� ���� ��� : /i �̸�
	Whisper,			//		�ӼӸ� : /w �̸� 
	MakeRoom,			//		�游��� : /make �ִ��ο� ������
	JoinRoom,			//		�� ���� : /join ���ȣ  
	Kick,				//		����(���常����) : /kick �̸�
	SendData,
	End
};

struct FUserInfo;
struct FRoomInfo {
	int					id;				// ����̵�
	int					name;			// ���̸�
	bool				isLobby;		// �κ��ΰ�?
	FUserInfo*			master;			// ���� ����
	set<FUserInfo*>		userInfoSet;	// �濡 �ִ� ���� ����

	FRoomInfo() {
		//master = new FUserInfo();
	}
	~FRoomInfo() {
		//delete master;
	}
};

struct FUserInfo {
	int					id;				// ���� ���̵�
	FString				name;			// ���� �̸�
	struct FRoomInfo*	roomInfo;		// ������ �ִ� �� ����

	FUserInfo() {
		//roomInfo = new FRoomInfo();
	}
	~FUserInfo() {
		//delete roomInfo;
	}
};

USTRUCT(Atomic, BlueprintType)
struct FPacket {

	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EPacketKind pk;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString data;
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

	void GotLogin(FString data);
	void GotSendData(const FString& data);

	FVector2D GetSizeBallon(FString data);

public:
	const int BUFSIZE = 1024;
	ushort	_writeOffset;
	uint8	_buf[1024];

	FSocket* _socket;
	bool _connected = false;

	FUserInfo* _userInfo;			// ���� ����

};
