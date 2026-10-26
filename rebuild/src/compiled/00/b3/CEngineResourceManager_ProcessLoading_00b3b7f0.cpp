// CEngineResourceManager::ProcessLoading(bool, double) -- per mesh bank housekeeping, then the graphic bank manager, every mesh bank's loading queue and the surface-composition manager process their loading until the given time
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B3B7F0, length 124 bytes (+ int3 padding).
// Signature from Ego_r.pdb: ?ProcessLoading@CEngineResourceManager@@QAEX_NN@Z (`ret 0Ch`: the bool's dword and the double).
// Callees: 0x009D68B0 the mesh bank's per-pass update (no arguments; the manifest's _Dest_val label is propagated),
// 0x009A4EC0 GFGetSystemManager() (its graphic bank manager at +0x6c), 0x009F8230 CGraphicBankManager::ProcessLoading(bool,
// double), 0x00A23B00 CMeshDataBank::ProcessLoadingQueue(bool, double), 0x00B2AD90
// CEngineSurfaceCompositionManager::ProcessLoading(bool, double) on the global @ 0x01436EAC (null-tested).
// Layout: engine/CEngineResourceManager.h (PDB): MeshBanks[3] at +0x10.
// Shape: two counted loops over the three bank pointers (`mov ebx,3 ... dec ebx ; jne` through edi, then `mov edi,3`
// through esi), each bank null-tested; the double is re-loaded from its argument slot and pushed with `sub esp,8 ;
// fstp qword ptr [esp]` for every call, the flag's whole argument dword is re-pushed from ebx; the graphic bank
// manager is read through a system-manager local (retail calls GFGetSystemManager and loads the member into ecx
// before the argument pushes; a manager local goes through eax, the bare chain calls after the pushes).
#include "engine/CEngineResourceManager.h"

class CMeshDataBank {
public:
    void UpdateLoading();                                // 0x009D68B0
    void ProcessLoadingQueue(bool flag, double until);   // 0x00A23B00
};
class CGraphicBankManager {
public:
    void ProcessLoading(bool flag, double until);        // 0x009F8230
};
class CSystemManager { public:
    unsigned char        _pad[0x6c];
    CGraphicBankManager* GraphicBankManager;             // +0x6c
};
CSystemManager* __cdecl GFGetSystemManager();            // 0x009A4EC0
class CEngineSurfaceCompositionManager {
public:
    void ProcessLoading(bool flag, double until);        // 0x00B2AD90
};
extern CEngineSurfaceCompositionManager* g_pSurfaceComposition_01436eac;   // 0x01436EAC

struct CEngineResourceManager_Methods : CEngineResourceManager {
    void ProcessLoading(bool flag, double until);
};

void CEngineResourceManager_Methods::ProcessLoading(bool flag, double until)
{
    long i;
    for (i = 0; i < 3; ++i) {
        if (MeshBanks[i])
            MeshBanks[i]->UpdateLoading();
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