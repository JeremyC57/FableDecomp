// CEngineWaterRenderer::ProcessSeaLoading() -- advance the asynchronous load of the requested sea body: drop everything when no sea is requested, release the old sea when the request changes, start the bank-file read of the requested sea, and load it once the read has finished
// Shape: FableWin.pdb / Ego_d line tables (debug source lines 585..655) -- empty-request block returns false; else-if (!HaveBackgroundSea || Requested !=
//   Loaded) block holding {smap_index} and {stream} scopes (Ego_d locals); one final `return false`. ReleaseSeaBuffers is the same-TU plain
//   member defined above the row in the original object (Ego_d TU order: LoadSea, ReleaseSeaBuffers, ..., CloseStaticMap, ProcessSeaLoading);
//   its text is the landed 0x00B6D480 row's. The six pointer drops are `Reset(NULL)` (FableWin lines 591/592, 613/614, 644/645 are
//   16 bytes each: push 0, member address, call -- ReleaseData() would be 14); with ReleaseData() the row needs /d2inlT196.
// Leaf: ?ProcessSeaLoading@CEngineWaterRenderer@@AAE_NXZ
// Retail: 0x00B6DC40, 771 bytes, thiscall.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHARSTRING_CONCAT_STRING
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_STLP_SET
#define FABLE_RETAIL_CARRAY
#define FABLE_RETAIL_CCOUNTEDPOINTER_DTOR_DECL_ONLY
#define FABLE_RETAIL_CCOUNTEDPOINTER_ASSIGN_DECL_ONLY
#define FABLE_RETAIL_CENGINEWATERRENDERER
#define FABLE_RETAIL_PASS22_WATER_ORIGINAL_COMPLETE_OWNER
#define FABLE_RETAIL_CDATAINPUTSTREAM
#define FABLE_RETAIL_CMEMORYDATAINPUTSTREAM
#define FABLE_RETAIL_CSTATICMAPBANKFILE_CLASS
#define FABLE_RETAIL_CBANKFILE_FINDINDEX
#define FABLE_RETAIL_CBANKFILE_ENTRYDATASIZE
#define FABLE_RETAIL_STATICBLOCKS_TEXTURE_HELPERS
#define FABLE_RETAIL_CBANKFILEASYNC_GETASYNCENTRY
#define FABLE_RETAIL_CBANKFILEASYNCENTRY_WATER
#define FABLE_RETAIL_CBANKFILEASYNCDATA_WATER
#include "fable_string.h"
#include "engine/CEngineWaterRenderer.h"
#include "engine/CBankFileAsyncEntry.h"
#include "engine/CBankFileAsyncData.h"
#include "engine/CStaticMapBankFile.h"
#include "engine/CMemoryDataInputStream.h"

extern EThreadedFilePriority g_HighLoadingPriority;

void CEngineWaterRenderer::ReleaseSeaBuffers()
{
    SeaBackgroundVertexBuffer.Uninitialise();
    SeaBackgroundVertexCount = 0;
    SeaBackgroundIndexBuffer.Uninitialise();
    SeaBackgroundPolyCount = 0;
    HaveBackgroundSea = false;
}

bool CEngineWaterRenderer::ProcessSeaLoading()
{
    if (RequestedSeaBodyID.IsEmpty()) {
        if (LoadingSea.Get()) {
            CBankFileAsyncData::DiscardAsyncData(LoadingSea);
            LoadingSea.Reset(NULL);
            LoadingSeaEntry.Reset(NULL);
        }
        if (HaveBackgroundSea) {
            ReleaseSeaBuffers();
        }
        return false;
    } else if (!HaveBackgroundSea || RequestedSeaBodyID != LoadedOrLoadingSeaBodyID) {
        if (HaveBackgroundSea) {
            ReleaseSeaBuffers();
        }
        if (RequestedSeaBodyID != LoadedOrLoadingSeaBodyID) {
            if (LoadingSea.Get()) {
                CBankFileAsyncData::DiscardAsyncData(LoadingSea);
                LoadingSea.Reset(NULL);
                LoadingSeaEntry.Reset(NULL);
            }
        }
        if (!StaticMapBankFile) {
            return false;
        }
        if (!LoadingSea.Get()) {
            unsigned long smap_index = StaticMapBankFile->FindIndexBySymbol(StaticMapSeaEntryPrefix + RequestedSeaBodyID);
            if (smap_index) {
                LoadedOrLoadingSeaBodyID = RequestedSeaBodyID;
                LoadingSeaEntry = StaticMapBankFile->GetAsyncEntry(smap_index);
                LoadingSea = LoadingSeaEntry->BeginReadDataAutoAlloc(0, LoadingSeaEntry->GetDataSize(), g_HighLoadingPriority);
                return true;
            } else {
                return false;
            }
        }
        if (LoadingSea->IsFinished()) {
            CMemoryDataInputStream stream(LoadingSea->GetData(), LoadingSea->GetLength());
            LoadSea(stream);
            LoadingSea.Reset(NULL);
            LoadingSeaEntry.Reset(NULL);
            return false;
        } else {
            return true;
        }
    }
    return false;
}