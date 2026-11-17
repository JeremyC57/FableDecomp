// CEngineResourceManager::ProcessLoading(bool, double) -- per mesh bank housekeeping, then the graphic bank manager, every mesh bank's loading queue and the surface-composition manager process their loading until the given time
// Leaf: ?ProcessLoading@CEngineResourceManager@@QAEX_NN@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B3B7F0, length 124 bytes (+ int3 padding).
// Signature from Ego_r.pdb: ?ProcessLoading@CEngineResourceManager@@QAEX_NN@Z (`ret 0Ch`: the bool's dword and the double).
// Callees: 0x009D68B0 CBankFileAsync::AsyncUpdatePending on each mesh bank,
// 0x009A4EC0 GFGetSystemManager() (its graphic bank manager at +0x6c), 0x009F8230 CGraphicBankManager::ProcessLoading(bool,
// double), 0x00A23B00 CMeshDataBank::ProcessLoadingQueue(bool, double), 0x00B2AD90
// CEngineSurfaceCompositionManager::ProcessLoading(bool, double) on the global @ 0x01436EAC (null-tested).
// Layout: engine/CEngineResourceManager.h (PDB): MeshBanks[3] at +0x10.
// Shape: two counted loops over the three bank pointers (`mov ebx,3 ... dec ebx ; jne` through edi, then `mov edi,3`
// through esi), each bank null-tested; the double is re-loaded from its argument slot and pushed with `sub esp,8 ;
// fstp qword ptr [esp]` for every call, the flag's whole argument dword is re-pushed from ebx; the graphic bank
// manager is read through a system-manager local (retail calls GFGetSystemManager and loads the member into ecx
// before the argument pushes; a manager local goes through eax, the bare chain calls after the pushes).
// Name (round 7, NF53): the mesh bank's per-pass update 0x009D68B0 is ?AsyncUpdatePending@CBankFileAsync@@QAEXXZ (Ego_r 0x00B2ABD0 /
//   FableWin 0x02FF6B50; CMeshDataBank : CBankFileAsync, rtti_bases.tsv) and 0x00A23B00 ?ProcessLoadingQueue@CMeshDataBank@@QAEX_NN@Z
//   (Ego_r 0x00B5FD80 / FableWin 0x031333B0); the self is the CEngineResourceManager member itself, not a derived _Methods view. Was a local
//   CMeshDataBank model (UpdateLoading) and the CEngineResourceManager_Methods view; now engine/CEngineResourceManager.h retail branch with
//   engine/CMeshDataBank.h (BASES + CTOR) and the overlay opt-ins _PROCESSLOADING_UNTIL, _PROCESSLOADINGQUEUE, _ASYNCUPDATEPENDING.
//   The local CSystemManager view (+0x6c) and the address-named 0x01436EAC global are unchanged.
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_STATICBLOCKS_TEXTURE_HELPERS
#define FABLE_RETAIL_CBANKFILEASYNC_ASYNCUPDATEPENDING
#define FABLE_RETAIL_CMESHDATABANK_BASES
#define FABLE_RETAIL_CMESHDATABANK_CTOR
#define FABLE_RETAIL_CMESHDATABANK_PROCESSLOADINGQUEUE
#define FABLE_RETAIL_CENGINERESOURCEMANAGER
#define FABLE_RETAIL_CENGINERESOURCEMANAGER_PROCESSLOADING_UNTIL
#include "fable_string.h"
#include "engine/CEngineResourceManager.h"
#include "engine/CMeshDataBank.h"

class CGraphicBankManager {
public:
    void ProcessLoading(bool flag, double until);        // 0x009F8230
};
class CSystemManager { public:
    unsigned char        _pad[0x6c];
    CGraphicBankManager* GraphicBankManager;             // +0x6c
};
CSystemManager* __fastcall GFGetSystemManager();            // 0x009A4EC0
class CEngineSurfaceCompositionManager {
public:
    void ProcessLoading(bool flag, double until);        // 0x00B2AD90
};
extern CEngineSurfaceCompositionManager* g_pSurfaceComposition_01436eac;   // 0x01436EAC

void CEngineResourceManager::ProcessLoading(bool flag, double until)
{
    long i;
    for (i = 0; i < 3; ++i) {
        if (MeshBanks[i])
            MeshBanks[i]->AsyncUpdatePending();
    }
    CSystemManager* system = GFGetSystemManager();
    system->GraphicBankManager->ProcessLoading(flag, until);
    for (i = 0; i < 3; ++i) {
        if (MeshBanks[i])
            MeshBanks[i]->ProcessLoadingQueue(flag, until);
    }
    if (g_pSurfaceComposition_01436eac)
        g_pSurfaceComposition_01436eac->ProcessLoading(flag, until);
}