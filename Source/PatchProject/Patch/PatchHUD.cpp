// Fill out your copyright notice in the Description page of Project Settings.


#include "PatchHUD.h"

#include "Kismet/GameplayStatics.h"

void UPatchHUD::InitHUD()
{
	if (PatchStartBtn->OnClicked.IsAlreadyBound(this, &UPatchHUD::OnPatchStartBtnClick) == false)
	{
		PatchStartBtn->OnClicked.AddDynamic(this, &UPatchHUD::OnPatchStartBtnClick);
	}
	if (GameStartBtn->OnClicked.IsAlreadyBound(this, &UPatchHUD::OnGameStartBtnClick) == false)
	{
		GameStartBtn->OnClicked.AddDynamic(this, &UPatchHUD::OnGameStartBtnClick);
	}
}

void UPatchHUD::OnPatchStartBtnClick()
{
	PatchingPanel->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	NonPatchingPanel->SetVisibility(ESlateVisibility::Collapsed);

	PatchStartEvent();
}

void UPatchHUD::OnGameStartBtnClick()
{
	UGameplayStatics::OpenLevel(this, "TopDownExampleMap");
}