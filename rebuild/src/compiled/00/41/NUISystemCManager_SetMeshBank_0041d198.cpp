// NUISystem::CManager::SetMeshBank(CCountedPointer<CMeshDataBank> mesh_bank) -- stores the shared mesh bank pointer
// Leaf: ?SetMeshBank@CManager@NUISystem@@QAEXV?$CCountedPointer@VCMeshDataBank@@@@@Z
// Retail: Fable.exe @ 0x0041D198, 28 bytes, `ret 8` (the counted pointer is passed by value and destroyed by the callee).
// Body: PMeshBank = mesh_bank -> CCountedPointer<CMeshDataBank>::ShareData(Data, Info) (0x004291B8) on the member, then the by-value
// parameter's destructor, expanded to its out-of-line ReleaseData<CMeshDataBank> (0x0042918C).
// NF221 (family q12226 / q12402): was ?SetGraphicsBank@CManager@NUISystem@@QAEXV?$CCountedPointer_0042918c@VCGraphicDataBank@@@@@Z
//   on a local CCountedPointer_0042918c<CGraphicDataBank> stand-in. Name and T: FableWin.pdb / Ego_r.pdb
//   ?SetMeshBank@CManager@NUISystem@@QAEXV?$CCountedPointer@VCMeshDataBank@@@@@Z (the four landed callers 0x0041E641, 0x004A6AB0,
//   0x004A6E30, 0x005958F5 bind it), the callees ShareData / ReleaseData<CMeshDataBank> (FableWin.pdb names, their own rows), and the
//   member: Ego_r.pdb PMeshBank (CCountedPointer<CMeshDataBank>) right after PGraphicsBank (+0x20 / +0x28 there; retail +0x10 / +0x18).
//   The counted pointer is now the shared engine/CCountedPointer.h (RELEASEDATA_DECL_ONLY + SHAREDATA_DECL_ONLY: both callees out of line).
// Layout: retail PMeshBank at +0x18; engine/NUISystem_CManager.h does not reproduce the retail offsets here (the STLport maps ahead of it
// are smaller in retail), so the class stays the HEAD row's local model (members respelled in place).
// Opt: size TU (`push dword ptr [esp+8]` forwarding twice).
#pragma optimize("s", on)
#define FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA_DECL_ONLY
#define FABLE_RETAIL_CCOUNTEDPOINTER_SHAREDATA_DECL_ONLY
#include "engine/CCountedPointer.h"
class CMeshDataBank;
namespace NUISystem {
class CManager {
public:
    void*                             __vftable;                 // +0x00
    unsigned char                     _pad04[0x14];              // +0x04
    CCountedPointer<CMeshDataBank>    PMeshBank;                 // +0x18
    void SetMeshBank(CCountedPointer<CMeshDataBank> mesh_bank);
};
void CManager::SetMeshBank(CCountedPointer<CMeshDataBank> mesh_bank)
{
    PMeshBank = mesh_bank;
}
}
