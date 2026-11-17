// CEngineResourceManager::FrameEnd() -- process loading until now plus the frame budget at +0x2aa0, release the three pools at +0x10..+0x18 (each when present), then the graphic data bank at +0x954 reloads with 3
// Leaf: ?FrameEnd@CEngineResourceManager@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B3B870, length 73 bytes.
// Callees: GFGetTime @ 0x009E1BC0 (a double in st0), ProcessLoading @ 0x00B3B7F0 (on `this`: the flag 1 and the double), the pool release @ 0x00A23AD0 (the manifest
// label is a propagated _Dest_val symbol), the bank's Reload @ 0x009FD100 (3). Shape: the double is added straight from memory (`fadd qword ptr [edi+2AA0h]`)
// and stored into the reserved 8-byte argument slot; the three pointers are walked with a counted loop (`mov ebx,3 ; ... ; dec ebx ; jne`) through esi.
// NF56 (round 7, queue q10476): moved onto the shared headers: engine/CEngineResourceManager.h (overlay opt-in
//   FABLE_RETAIL_CENGINERESOURCEMANAGER_FRAMEEND for Ego_r.pdb 0x009D9ED0 / FableWin.pdb 0x02DD3B60 ?FrameEnd@CEngineResourceManager@@QAEXXZ;
//   MeshBanks +0x10, GraphicBanks +0x954, MaxLoadingTime +0x2AA0), so the call binds ?ProcessLoading@CEngineResourceManager@@QAEX_NN@Z
//   (0x00B3B7F0, the bool flag), the pool release is ?FrameEnd@CMeshDataBank@@QAEXXZ (0x00A23AD0, engine/CMeshDataBank.h) and the bank call
//   CGraphicDataBank::SetReduceSizeFrameDelay; GFGetTime from engine/GFGetTime.h. Were a local CEngineResourceManager model with
//   ProcessLoading(long, double), the stand-in CResourcePool_00a23ad0::Release and a local CGraphicDataBank.
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_STATICBLOCKS_TEXTURE_HELPERS
#define FABLE_RETAIL_CMESHDATABANK_BASES
#define FABLE_RETAIL_CMESHDATABANK
#define FABLE_RETAIL_CENGINERESOURCEMANAGER
#define FABLE_RETAIL_CENGINERESOURCEMANAGER_PROCESSLOADING_UNTIL
#define FABLE_RETAIL_CENGINERESOURCEMANAGER_FRAMEEND
#define FABLE_RETAIL_CGRAPHICDATABANK_ORIGINAL_METHODS_PASS22
#include "fable_string.h"
#include "engine/GFGetTime.h"
#include "engine/CEngineResourceManager.h"
#include "engine/CMeshDataBank.h"
class CDataInputStream; class CDataOutputStream;
#include "engine/CGraphicDataBank.h"

void CEngineResourceManager::FrameEnd()
{
    ProcessLoading(true, GFGetTime() + MaxLoadingTime);
    for (long i = 0; i < 3; ++i) {
        if (MeshBanks[i])
            MeshBanks[i]->FrameEnd();
    }
    GraphicBanks[0]->SetReduceSizeFrameDelay(3);
}
