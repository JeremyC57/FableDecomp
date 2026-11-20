// NF131 (2026-11-20, q6182): the header block's stream is an ordinary local built from GetBlockData(id) and the block size, as
//   retail does (`push [esi+34h]` size ; `push [esi+30h]` id ; call 0x00BE04C0 ; `push eax` ; call 0x00994930, `ret 8` popping both).
//   The old form (a two-argument GetBlockData, a one-argument ctor placed at +4 into a byte buffer and an explicit dtor call) gave the
//   same bytes under two wrong spellings. Now binds ??0CMemoryDataInputStream@@QAE@PBXK@Z (Ego_r.pdb 0x00AFCA20 + FableWin.pdb
//   0x030849C0) and ?GetBlockData@CEngineStaticMapFileBlockManager@@QBEPBEJ@Z (Ego_r 0x00A41E20 + FableWin 0x02DF74B0) through
//   the candidates header's opt-ins below.
#define FABLE_RETAIL_CENGINESTATICMAPFILEBLOCKMANAGER_GETBLOCKDATA
#define FABLE_RETAIL_CMEMORYDATAINPUTSTREAM_LOD_CTOR
#include <new>
#include "candidates/landscape_background_lod_compiled.h"

LS_LOADING_STATUS CEngineLandscapeMap::UpdateBackgroundLOD(
    CLandscapeGPUUpdateQueue&,
    double distanceToCamera,
    bool forceUpdate)
{
    LS_LOADING_STATUS status = LS_LOADING_READY;
    LandscapeUpdateActive = true;

    if (BackgroundTree == 0)
    {
        if (BackgroundLoadEntry != 0)
        {
            if (!BackgroundHeaderRequestIssued)
            {
                StaticMapFileBlockManager->AddRequest(
                    BackgroundHeaderBlockId, BackgroundHeaderBlockSize, false);
                BackgroundHeaderRequestIssued = true;
            }
            if (StaticMapFileBlockManager->IsBlockLoaded(BackgroundHeaderBlockId))
            {
                void* storage = AllocateLandscapeBackgroundTreeNode(0xE8, this);
                BackgroundTree = storage != 0 ?
                    new(storage) CLandscapeBackgroundTreeNode(this, 0, LandscapeMapIndex) : 0;
                if (BackgroundTree != 0)
                {
                    CMemoryDataInputStream stream(StaticMapFileBlockManager->GetBlockData(BackgroundHeaderBlockId),
                        BackgroundHeaderBlockSize);
                    BackgroundTree->LoadHeader(stream);
                    ReleaseFileBlockLoad();
                }
            }
            else
            {
                status = LS_LOADING_PENDING;
            }
        }
        else if (OwnerMap->EdgeHeightFile == 0)
        {
            void* storage = AllocateLandscapeBackgroundTreeNode(0xE8, this);
            BackgroundTree = storage != 0 ?
                new(storage) CLandscapeBackgroundTreeNode(this, 0, LandscapeMapIndex) : 0;
            BackgroundTree->BuildDefaultSection(
                0, 0, OwnerMap->BackgroundWidth, OwnerMap->BackgroundHeight);
        }
    }

    if (BackgroundTree != 0 && !ForegroundLODUpdateActive)
        status = BackgroundTree->UpdateStaticLOD(
            distanceToCamera, forceUpdate, StaticMapFileBlockManager);

    if (StaticMapFileBlockManager != 0)
        StaticMapFileBlockManager->BankFileList_RemoveFinished();
    LandscapeUpdateActive = false;
    return status;
}
