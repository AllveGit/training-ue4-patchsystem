// Fill out your copyright notice in the Description page of Project Settings.


#include "PatchProjectGameInstance.h"
#include "PatchSystem.h"

void UPatchProjectGameInstance::Init()
{
	Super::Init();

	if (PatchSystem == nullptr)
	{
		PatchSystem = NewObject<UPatchSystem>(UPatchSystem::StaticClass());
		PatchSystem->Initialize("Windows");
	}
}

void UPatchProjectGameInstance::Shutdown()
{
	Super::Shutdown();

	if (PatchSystem)
	{
		PatchSystem->Shutdown();
		PatchSystem = nullptr;
	}
}

UPatchSystem* UPatchProjectGameInstance::GetPatchSystem()
{
	if (PatchSystem == nullptr)
	{
		PatchSystem = NewObject<UPatchSystem>(UPatchSystem::StaticClass());
		PatchSystem->Initialize("Windows");
	}

	return PatchSystem;
}