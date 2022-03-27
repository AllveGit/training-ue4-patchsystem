// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "PatchProjectGameInstance.generated.h"

class UPatchSystem;

/**
 * 
 */
UCLASS()
class PATCHPROJECT_API UPatchProjectGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	virtual void Init() override;
	virtual void Shutdown() override;

	UPatchSystem* GetPatchSystem();
private:
	UPROPERTY(Transient)
	UPatchSystem* PatchSystem = nullptr;
};
