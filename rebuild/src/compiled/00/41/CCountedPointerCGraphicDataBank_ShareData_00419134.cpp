#pragma optimize("s", on)
// CCountedPointer<CGraphicDataBank>::ShareData(CGraphicDataBank* data, CCPPointerInfo* info) -- adopt another pointer's count block: if it differs, release ours (ReleaseData 0x00419108), take Data / Info and bump the count
// Leaf: ?ShareData@?$CCountedPointer@VCGraphicDataBank@@@@QAEXPAVCGraphicDataBank@@PAVCCPPointerInfo@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00419134, 38 bytes, thiscall `ret 8`. Callee: 0x00419108.
// Identity (NF221, family q12226 / q12402): FableWin.pdb ?ShareData@?$CCountedPointer@VCGraphicDataBank@@@@QAEXPAVCGraphicDataBank@@PAVCCPPointerInfo@@@Z; callers 0x004194BA (operator=<CGraphicDataBank>) and 0x0042A9B7 (NUISystem::CManager::SetGraphicsBank).
// Was ?ShareData@CCPtr@@QAEXPAXPAJ@Z (local CCPtr stand-in, ReleaseCur_00419108); ported onto engine/CCountedPointer.h (FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA_DECL_ONLY: the callee is out of line in this size TU).
#define FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA_DECL_ONLY
#include "engine/CCountedPointer.h"
class CGraphicDataBank;
template void CCountedPointer<CGraphicDataBank>::ShareData(CGraphicDataBank*, CCPPointerInfo*);
