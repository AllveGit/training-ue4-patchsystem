#pragma once

#include "CoreMinimal.h"
#include "Runtime/Online/HTTP/Public/Http.h"
#include "PatchSystem.generated.h"

UCLASS()
class PATCHSYSTEM_API UPatchSystem : public UObject
{
	GENERATED_BODY()

public:
	void Initialize();
	void Shutdown();
public:
	UPROPERTY(EditDefaultsOnly, Category = "Patching")
	FString PatchVersionURL;
private:
	TArray<int32> SingleChunkDownloadList;
	bool bIsDownloadingSingleChunks = false;
};