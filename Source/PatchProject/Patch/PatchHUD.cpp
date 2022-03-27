// Fill out your copyright notice in the Description page of Project Settings.


#include "PatchHUD.h"
#include "PatchProject/PatchProjectGameInstance.h"
#include "PatchSystem.h"
#include "Kismet/GameplayStatics.h"

void UPatchHUD::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (PatchSystem.IsValid())
	{
	}
}

void UPatchHUD::InitHUD()
{
	auto gameInstance = Cast<UPatchProjectGameInstance>(GetWorld()->GetGameInstance());
	PatchSystem = gameInstance->GetPatchSystem();

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
	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("GameStart Btn Click!"));

	UWorld* World = GEngine->GetWorldFromContextObject(this, EGetWorldErrorMode::LogAndReturnNull);
	if (World == nullptr)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("World is null!"));
		return;
	}

	UGameplayStatics::OpenLevel(this, "TopDownExampleMap");
}