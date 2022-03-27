#pragma once

#include "CoreMinimal.h"
#include "Runtime/Online/HTTP/Public/Http.h"
#include "PatchSystem.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FPatchCompleteDelegate, bool, bSuccess);

/** 패치 다운로드 상태를 알려주는 정보들을 모아놓은 구조체 */
USTRUCT(BlueprintType)
struct FPatchDownloadInfo
{
	GENERATED_BODY()

public:
	FPatchDownloadInfo()
	{
	}

	FPatchDownloadInfo(FText InErrorText, int32 InFileDownloaded, int32 InTotalFileDownload, int32 InMBDownloaded, int32 InTotalMBDownload, float InDownloadPer) :
		DownloadErrorText(InErrorText),
		FilesDownloaded(InFileDownloaded),
		TotalFilesToDownload(InTotalFileDownload),
		MBDownloaded(InMBDownloaded),
		TotalMBToDownload(InTotalMBDownload),
		DownloadPercent(InDownloadPer)
	{
	}
public:
	FText DownloadErrorText;

	int32 FilesDownloaded;
	int32 TotalFilesToDownload;

	int32 MBDownloaded;
	int32 TotalMBToDownload;

	float DownloadPercent;
};

UCLASS()
class PATCHSYSTEM_API UPatchSystem : public UObject
{
	GENERATED_BODY()

public:
	void Initialize(const FString& InPatchPlatform);
	void Shutdown();

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/** Patch Readying */
	void OnPatchVersionResponse(FHttpRequestPtr InRequest, FHttpResponsePtr InResponse, bool bInSuccess);
	void OnPatchManifestUpdateComplete(bool bInSuccess);
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/** Patch Progressing */
	void ProcessPatch();
	void OnPatchMountChunkComplete(bool bInSuccess);
	FPatchDownloadInfo GetPatchDownloadInfo() const;
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/** Delegates */
	FPatchCompleteDelegate OnPatchReady;
	FPatchCompleteDelegate OnPatchComplete;
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
private:
	/** TODO : 정보추가 필요, 배포할때 ini 키 머시기머시기.. */
	const FString DeploymentName = "Patching-Live";

	/** 동시에 몇개를 다운받을 수 있는지 */
	const int32 ConcurrentAllowDownloadNum = 8;

	/** 패치 매니페스트의 갱신이 확인되었는지 확인 */
	bool bIsDownloadManifestUpToDate;

	/** 패치 진행중인지 상태 확인 */
	bool bIsPatchProcessing;

	/** 패치 버전을 확인할 서버주소 */
	FString PatchVersionURL;

	/** 패치할 플랫폼 이름 (ex. Windows, Android, IOS...) */
	FString PatchPlatform;

	/** 다운받을 청크의 ID들 */
	TArray<int32> ChunkDownloadIDs;
};