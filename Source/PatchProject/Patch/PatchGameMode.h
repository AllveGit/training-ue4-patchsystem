// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Blueprint/UserWidget.h"
#include "PatchGameMode.generated.h"

class UPatchHUD;

UCLASS()
class PATCHPROJECT_API APatchGameMode : public AGameModeBase
{
	GENERATED_BODY()
public:
	APatchGameMode();

	virtual void StartPlay() override;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> PatchHUDClass;
private:
	UPROPERTY(Transient)
	UPatchHUD* PatchHUD = nullptr;
};
