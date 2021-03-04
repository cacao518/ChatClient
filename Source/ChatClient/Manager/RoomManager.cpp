// Fill out your copyright notice in the Description page of Project Settings.


#include "RoomManager.h"
#include <string>
#include <vector>
#include "Engine/World.h"
#include "../Manager/MyGameInstance.h"

RoomManager::RoomManager()
{
}

RoomManager::~RoomManager()
{
}

void RoomManager::JoinRoom(int id)
{
}

void RoomManager::UpdateCurUserInfo(const string & data)
{
	for (auto userInfo : _curUserSet)
		delete userInfo;
	
	_curUserSet.clear();

	string str = data;
	while (1)
	{
		auto idOffset = str.find('(');
		if (idOffset == string::npos) break;

		string id_s = str.substr(0, idOffset);
		str.erase(0, idOffset + 1);
		int id = atoi(id_s.c_str());

		auto nameOffset = str.find(')');
		string name = str.substr(0, nameOffset);
		str.erase(0, nameOffset + 1);
		name += '\0';

		// 멀티바이트 -> 유니코드로 변환
		wchar_t name_uni[1024];
		mbstowcs(name_uni, name.c_str(), name.size());
	
		FString finalName(name_uni);

		
		// 현재 방에 있는 유저 정보에 저장
		_curUserSet.insert( new FUserInfo{id , finalName, 0, FString("")} );
	}
}

void RoomManager::UpdateAllRoomInfo(const string & data)
{
	_id_roomMap.clear();

	string str = data;
	while (1)
	{
		auto idOffset = str.find('(');
		if (idOffset == string::npos) break;

		string id_s = str.substr(0, idOffset);
		str.erase(0, idOffset + 1);
		int id = atoi(id_s.c_str());

		auto nameOffset = str.find(')');
		string name = str.substr(0, nameOffset);
		str.erase(0, nameOffset + 1);
		name += '\0';

		auto userNumOffset = str.find('(');
		string userNum_s = str.substr(0, userNumOffset);
		str.erase(0, userNumOffset + 1);
		int userNum = atoi(userNum_s.c_str());

		// 멀티바이트 -> 유니코드로 변환
		wchar_t name_uni[1024];
		mbstowcs(name_uni, name.c_str(), name.size());

		FString finalName(name_uni);


		_id_roomMap[id] = FRoomInfo{ id , finalName, userNum };
	}
}
