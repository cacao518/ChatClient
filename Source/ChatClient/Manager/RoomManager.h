// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include <unordered_map>
#include <set>
#include <string>
#include <vector>
#include "../ETC/TcpSocket.h"

/**
 * 
 */
struct FRoomInfo;
struct FUserInfo;

class CHATCLIENT_API RoomManager
{
public:
	typedef std::unordered_map<std::string, FRoomInfo> Name_RoomMap; // ���̸�, ������

	typedef std::unordered_map<int, FRoomInfo> ID_RoomMap; // ���ȣ, ������
	typedef std::set<FUserInfo*> curUserSet; // ����� ��������

	RoomManager();
	~RoomManager();

	void UpdateCurUserInfo(const string& data);
	void UpdateAllRoomInfo(const string& data);

	ID_RoomMap&			GetID_RoomMap() { return _id_roomMap; };
	Name_RoomMap&		GetName_RoomMap() { return _name_roomMap; };
	curUserSet&			GetCurUserSet() { return _curUserSet; };

private:

	Name_RoomMap _name_roomMap;
	ID_RoomMap _id_roomMap;

	curUserSet _curUserSet;
};
