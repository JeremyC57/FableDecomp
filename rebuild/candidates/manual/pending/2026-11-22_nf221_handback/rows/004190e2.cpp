#pragma optimize("s", on)
// CCountedPointer<CFontBank>::ShareData(CFontBank* data, CCPPointerInfo* info) -- adopt another pointer's count block: if it differs, release ours (ReleaseData 0x004057A0), take Data / Info and bump the count
// Leaf: ?ShareData@?$CCountedPointer@VCFontBank@@@@QAEXPAVCFontBank@@PAVCCPPointerInfo@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x004190E2, 38 bytes, thiscall `ret 8`. Callee: 0x004057A0.
// Identity (NF221, family q12226 / q12402): FableWin.pdb ?ShareData@?$CCountedPointer@VCFontBank@@@@QAEXPAVCFontBank@@PAVCCPPointerInfo@@@Z; caller 0x00419463 (operator=<CFontBank>); Ego_r.pdb also keeps ?ShareData@?$CCountedPointer@VCFontBank@@@@ out of line.
// Was ?ShareData@CCPtr@@QAEXPAXPAJ@Z (local CCPtr stand-in, ReleaseCur); ported onto engine/CCountedPointer.h (FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA_DECL_ONLY: the callee is out of line in this size TU).
#define FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA_DECL_ONLY
#include "engine/CCountedPointer.h"
class CFontBank;
template void CCountedPointer<CFontBank>::ShareData(CFontBank*, CCPPointerInfo*);
