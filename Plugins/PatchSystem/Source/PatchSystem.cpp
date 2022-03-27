#include "PatchSystem.h"
#include "ChunkDownloader.h"

DEFINE_LOG_CATEGORY_STATIC(LogPatch, Display, Display);

void UPatchSystem::Initialize(const FString& InPatchPlatform)
{
	PatchPlatform = InPatchPlatform;

	FHttpModule& http = FHttpModule::Get();

	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> request = http.CreateRequest();
	request->OnProcessRequestComplete().BindUObject(this, &UPatchSystem::OnPatchVersionResponse);

	// configure and send the request
	request->SetURL(PatchVersionURL);
	request->SetURL("GET");
	request->SetHeader(TEXT("User-Agent"), "X-UnrealEngine-Agent");
	request->SetHeader("Content-Type", TEXT("application/json"));
	request->ProcessRequest();
}

void UPatchSystem::Shutdown()
{
	// Shutdown the chunk downloader
	FChunkDownloader::Shutdown();
}

void UPatchSystem::OnPatchVersionResponse(FHttpRequestPtr InRequest, FHttpResponsePtr InResponse, bool bInSuccess)
{
	// 새로운 패치 버전이 서버로부터 온다면 업데이트 가능
	FString contentBuildID = InResponse->GetContentAsString();
	UE_LOG(LogPatch, Display, TEXT("Patch Content Build ID Response : %s"), *contentBuildID);

	// 플랫폼에 맞는 ChunkDownloader 설정 및 생성
	UE_LOG(LogPatch, Display, TEXT("Creating Chunk Downloader"));
	TSharedRef<FChunkDownloader> downloader = FChunkDownloader::GetOrCreate();
	downloader->Initialize(PatchPlatform, ConcurrentAllowDownloadNum);

	// 캐싱된 빌드 매니페스트 데이터가 있는 경우 로드한다
	// ChunkDownloader가 가장 최근에 다운로드된 매니페스트 데이터로 채워진다
	UE_LOG(LogPatch, Display, TEXT("Loading Cached Build ID"));
	if (downloader->LoadCachedBuild(DeploymentName))
	{
		UE_LOG(LogPatch, Display, TEXT("Cached Build Succeeded"));
	}
	else
	{
		UE_LOG(LogPatch, Display, TEXT("Cached Build Failed"));
	}

	// 매니페스트 업데이트가 끝난 후 상태를 설정하는 콜백함수
	TFunction<void(bool)> manifestUpdCompleteCallback = [&](bool bInSuccess) { OnPatchManifestUpdateComplete(bInSuccess); };

	// 서버에 새로운 매니페스트 파일이 있는지 확인 후 다운로드 하는 매니페스트 업데이트 시작
	downloader->UpdateBuild(DeploymentName, contentBuildID, manifestUpdCompleteCallback);
}

void UPatchSystem::OnPatchManifestUpdateComplete(bool bInSuccess)
{
	if (bInSuccess)
	{
		UE_LOG(LogPatch, Display, TEXT("Manifest Update Succeeded"));
	}
	else
	{
		UE_LOG(LogPatch, Display, TEXT("Manifest Update Failed"));
	}

	bIsDownloadManifestUpToDate = bInSuccess;

	// 여기까지 왔다면, 패치를 하기 위해 준비가 끝난 상태이다.
	OnPatchReady.Broadcast(bInSuccess);
}

void UPatchSystem::ProcessPatch()
{
	// 매니페스트가 최신 상태가 아니라면
	if (!bIsDownloadManifestUpToDate)
	{
		UE_LOG(LogPatch, Display, TEXT("Manifest Update Failed. Can't patch the game"));
		return;
	}

	bIsPatchProcessing = true;

	TSharedRef<FChunkDownloader> downloader = FChunkDownloader::GetChecked();
	
	// 청크들의 상태 (다운로드되었는지, 진행 중인지, 보류 중인지 등)을 로그로 남긴다.
	for (int32 chunkID : ChunkDownloadIDs)
	{
		int32 chunkStatus = static_cast<int32>(downloader->GetChunkStatus(chunkID));
		UE_LOG(LogPatch, Display, TEXT("chunk %i status: %i"), chunkID, chunkStatus);
	}

	TFunction<void(bool)> mountChunkCompleteCallback = [this](bool bInSuccess) { OnPatchMountChunkComplete(bInSuccess); };
	downloader->MountChunks(ChunkDownloadIDs, mountChunkCompleteCallback);
}

void UPatchSystem::OnPatchMountChunkComplete(bool bInSuccess)
{
	bIsPatchProcessing = false;

	if (bInSuccess)
	{
		UE_LOG(LogPatch, Display, TEXT("Chunk Mount Complete"));
		OnPatchComplete.Broadcast(true);
	}
	else
	{
		UE_LOG(LogPatch, Display, TEXT("Chunk Mount Failed"));
		OnPatchComplete.Broadcast(false);
	}
}

FPatchDownloadInfo UPatchSystem::GetPatchDownloadInfo() const
{
	// ChunkDownloader의 다운로딩 정보를 가져온다.
	TSharedRef<FChunkDownloader> downloader = FChunkDownloader::GetChecked();
	FChunkDownloader::FStats loadingStats = downloader->GetLoadingStats();

	// MiB의 바이트 수
	const uint64 byteFromMiB = (1024 * 1024);

	// 다운로드 정보 랩핑
	int32 filesDownloaded = (int32)loadingStats.FilesDownloaded;
	int32 totalFilesDownload = (int32)loadingStats.TotalFilesToDownload;
	int32 mbDownloaded = (int32)(loadingStats.BytesDownloaded / byteFromMiB);
	int32 totalMBDownload = (int32)(loadingStats.TotalBytesToDownload / byteFromMiB);
	float downloadPercent = (float)loadingStats.BytesDownloaded / (float)loadingStats.TotalBytesToDownload;

	FPatchDownloadInfo downloadInfo(loadingStats.LastError, filesDownloaded, totalFilesDownload, mbDownloaded, totalMBDownload, downloadPercent);
	return downloadInfo;
}