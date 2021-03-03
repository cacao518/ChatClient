// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include <unordered_map>
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
	typedef std::unordered_map<int, FRoomInfo> ID_RoomMap; // 방번호, 룸정보
	typedef std::unordered_map<std::string, FRoomInfo> Name_RoomMap; // 방이름, 룸정보
public:
	RoomManager();
	~RoomManager();

	//void AddRoom(int roomId, string roomName);
	void UpdateRoomInfo(std::vector<FRoomInfo>& roominfos);

	ID_RoomMap& GetID_RoomMap() { return _id_roomMap; };
	Name_RoomMap& GetName_RoomMap() { return _name_roomMap; };

private:
	ID_RoomMap _id_roomMap;
	Name_RoomMap _name_roomMap;
};
