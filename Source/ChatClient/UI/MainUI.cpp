// Fill out your copyright notice in the Description page of Project Settings.


#include "MainUI.h"
//#include "Components/Button.h"
#include "Components/EditableText.h"
#include "Blueprint/WidgetTree.h"
#include "Engine/World.h"
#include "../Manager/MyGameInstance.h"
//#include "Components/ScrollBox.h"
#include "Runtime/UMG/Public/UMG.h"
#include "../Manager/UIManager.h"
#include "../Level/LoignLevel.h"

void UMainUI::NativeConstruct()
{
	UMyGameInstance* gameInstance = Cast< UMyGameInstance>(GetGameInstance());
	if (gameInstance == nullptr) return;

	//UE_LOG(LogTemp, Warning, TEXT("Test"));
	_scrollBox = Cast<UScrollBox>(WidgetTree->FindWidget("scroll"));
	_sendBt = Cast<UButton>(WidgetTree->FindWidget("SendBt"));
	_editText = Cast<UEditableText>(WidgetTree->FindWidget("editText"));
	_userScroll = Cast<UScrollBox>(WidgetTree->FindWidget("userScroll"));

	_myUserNameText = Cast<UTextBlock>(WidgetTree->FindWidget("nameText"));
	_curRoomNameText = Cast<UTextBlock>(WidgetTree->FindWidget("roomNameText"));
	_curUserNumText = Cast<UTextBlock>(WidgetTree->FindWidget("userNumText"));

	_roomAddBt = Cast<UButton>(WidgetTree->FindWidget("menuBt1"));
	_roomListBt = Cast<UButton>(WidgetTree->FindWidget("menuBt2"));
	_exitBt = Cast<UButton>(WidgetTree->FindWidget("menuBt3"));

	_sendBt->OnClicked.AddDynamic(this, &UMainUI::OnClickedFunc_SendBt);
	_roomAddBt->OnClicked.AddDynamic(this, &UMainUI::OnClickedFunc_RoomAddBt);
	_roomListBt->OnClicked.AddDynamic(this, &UMainUI::OnClickedFunc_RoomListBt);
	_exitBt->OnClicked.AddDynamic(this, &UMainUI::OnClickedFunc_ExitBt);
}

// ä�� ���� ��ư 
void UMainUI::OnClickedFunc_SendBt()
{
	UMyGameInstance* gameInstance = Cast< UMyGameInstance>(GetGameInstance());

	if (gameInstance == nullptr) return;
	if (gameInstance->GetSocket()->GetisConnect() == false) return;
	if (_editText == nullptr) return;
	// ä�� ������
	FString editTextStr = _editText->GetText().ToString();
	if (gameInstance->GetSocket()->GetisConnect() == true)
		gameInstance->GetSocket()->Send( editTextStr );

	_editText->SetText(FText::FromString(""));

}

// �� ���� ��ư
void UMainUI::OnClickedFunc_RoomAddBt()
{
	UMyGameInstance* gameInstance = Cast< UMyGameInstance>(GetGameInstance());
	if (gameInstance == nullptr) return;

	// �� ����Ʈ ���� ����
	FString path = FString("/Game/2DSideScrollerCPP/Blueprints/BP_AddRoomUI.BP_AddRoomUI_C");
	UClass* roomAddUI = ConstructorHelpersInternal::FindOrLoadClass(path, UUserWidget::StaticClass());
	_roomAddUIWidget = CreateWidget<UUserWidget>(GetWorld(), roomAddUI);

	if (_roomAddUIWidget != nullptr)
	{
		_roomAddUIWidget->AddToViewport();
	}
}

// �� ��� ��ư
void UMainUI::OnClickedFunc_RoomListBt()
{
	UMyGameInstance* gameInstance = Cast< UMyGameInstance>(GetGameInstance());
	if (gameInstance == nullptr) return;

	// �� ����Ʈ ���� ����
	FString path = FString("/Game/2DSideScrollerCPP/Blueprints/BP_RoomList.BP_RoomList_C");
	UClass* roomListUI = ConstructorHelpersInternal::FindOrLoadClass(path, UUserWidget::StaticClass());
	_roomListUIWidget = CreateWidget<UUserWidget>(GetWorld(), roomListUI);
	
	if (_roomListUIWidget != nullptr)
	{
		_roomListUIWidget->AddToViewport();

		// ��� �� ���� �������
		FString sendData = L"/room";
		if (gameInstance->GetSocket()->GetisConnect() == true)
			gameInstance->GetSocket()->Send(sendData);
	}
}

// ������ ��ư
void UMainUI::OnClickedFunc_ExitBt()
{
	UMyGameInstance* gameInstance = Cast< UMyGameInstance>(GetGameInstance());
	if (gameInstance == nullptr) return;

	// ������
	FString sendData = L"/exit";
	if (gameInstance->GetSocket()->GetisConnect() == true)
		gameInstance->GetSocket()->Send(sendData);

	gameInstance->GetUIManager().GetLoginLevel()->CreateLoginUI();


}
