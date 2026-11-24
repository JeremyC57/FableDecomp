#pragma optimize("s", on)
// CCountedPointer<CMeshDataBank>::ShareData(CMeshDataBank* data, CCPPointerInfo* info) -- adopt another pointer's count block: if it differs, release ours (ReleaseData 0x0042918C), take Data / Info and bump the count
// Leaf: ?ShareData@?$CCountedPointer@VCMeshDataBank@@@@QAEXPAVCMeshDataBank@@PAVCCPPointerInfo@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x004291B8, 38 bytes, thiscall `ret 8`. Callee: 0x0042918C.
// Identity (NF221, family q12226 / q12402): FableWin.pdb ?ShareData@?$CCountedPointer@VCMeshDataBank@@@@QAEXPAVCMeshDataBank@@PAVCCPPointerInfo@@@Z; caller 0x0041D198 (NUISystem::CManager::SetMeshBank, which takes a CCountedPointer<CMeshDataBank>: FableWin.pdb ?SetMeshBank@CManager@NUISystem@@QAEXV?$CCountedPointer@VCMeshDataBank@@@@@Z).
// Was ?ShareData@CCPtr@@QAEXPAXPAJ@Z (local CCPtr stand-in, ReleaseCur_0042918c); ported onto engine/CCountedPointer.h (FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA_DECL_ONLY: the callee is out of line in this size TU).
#define FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA_DECL_ONLY
#include "engine/CCountedPointer.h"
class CMeshDataBank;
template void CCountedPointer<CMeshDataBank>::ShareData(CMeshDataBank*, CCPPointerInfo*);
