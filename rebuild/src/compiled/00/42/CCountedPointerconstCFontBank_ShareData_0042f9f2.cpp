#pragma optimize("s", on)
// CCountedPointer<const CFontBank>::ShareData(const CFontBank* data, CCPPointerInfo* info) -- adopt another pointer's count block: if it differs, release ours (ReleaseData 0x0042F948), take Data / Info and bump the count
// Leaf: ?ShareData@?$CCountedPointer@$$CBVCFontBank@@@@QAEXPBVCFontBank@@PAVCCPPointerInfo@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0042F9F2, 38 bytes, thiscall `ret 8`. Callee: 0x0042F948.
// Identity (NF221, family q12226 / q12402): FableWin.pdb ?ShareData@?$CCountedPointer@$$CBVCFontBank@@@@QAEXPBVCFontBank@@PAVCCPPointerInfo@@@Z; caller 0x0042FE34 (operator=<CFontBank> of CCountedPointer<const CFontBank>, FableWin.pdb member template).
// Was ?ShareData@CCPtr@@QAEXPAXPAJ@Z (local CCPtr stand-in, ReleaseCur_0042f948); ported onto engine/CCountedPointer.h (FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA_DECL_ONLY: the callee is out of line in this size TU).
#define FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA_DECL_ONLY
#include "engine/CCountedPointer.h"
class CFontBank;
template void CCountedPointer<const CFontBank>::ShareData(const CFontBank*, CCPPointerInfo*);
