// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/CanvasPanel.h"
#include "Components/Button.h"
#include "PatchHUD.generated.h"

/**
 * 
 */
UCLASS()
class PATCHPROJECT_API UPatchHUD : public UUserWidget
{
	GENERATED_BODY()

public:
	void InitHUD();

	UFUNCTION(BlueprintImplementableEvent)
	void PatchStartEvent();

	UFUNCTION()
	void OnPatchStartBtnClick();
	UFUNCTION()
	void OnGameStartBtnClick();
protected:
	UPROPERTY(meta = (BindWidget))
	UCanvasPanel* PatchingPanel = nullptr;
	UPROPERTY(meta = (BindWidget))
	UCanvasPanel* NonPatchingPanel = nullptr;

	UPROPERTY(meta = (BindWidget))
	UButton* PatchStartBtn = nullptr;
	UPROPERTY(meta = (BindWidget))
	UButton* GameStartBtn = nullptr;
};
