// Fill out your copyright notice in the Description page of Project Settings.


#include "PatchGameMode.h"
#include "PatchHUD.h"

APatchGameMode::APatchGameMode()
{

}

void APatchGameMode::StartPlay()
{
	Super::StartPlay();

	if (PatchHUDClass != nullptr)
	{
		PatchHUD = CreateWidget<UPatchHUD>(GetWorld(), PatchHUDClass);
		PatchHUD->InitHUD();
		PatchHUD->AddToPlayerScreen();
	}
}