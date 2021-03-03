// Fill out your copyright notice in the Description page of Project Settings.


#include "UIManager.h"
#include "Engine/World.h"
#include "Blueprint/UserWidget.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/TextBlock.h"
#include "Blueprint/WidgetTree.h"
#include "LoginUI.h"
#include "MyGameInstance.h"
#include "Engine/GameInstance.h"

UIManager::UIManager()
{
	
}

UIManager::~UIManager()
{
}

void UIManager::ShowToast(FString & text)
{
	if (_gameInstance == nullptr) return;

	FString path = FString("/Game/2DSideScrollerCPP/Blueprints/BP_Toast.BP_Toast_C");
	UClass* toastUI = ConstructorHelpersInternal::FindOrLoadClass(path, UUserWidget::StaticClass());
	UUserWidget* toastWidget = CreateWidget<UUserWidget>(_gameInstance->GetWorld(), toastUI);

	if (toastWidget != nullptr)
	{
		toastWidget->AddToViewport();
		UTextBlock* _toastText = Cast<UTextBlock>(toastWidget->WidgetTree->FindWidget("ToastText"));
		_toastText->SetText(FText::AsCultureInvariant(text));
	}

}

void UIManager::SetGameInstance()
{
	_gameInstance = UMyGameInstance::GetMyGameInstance();
}
