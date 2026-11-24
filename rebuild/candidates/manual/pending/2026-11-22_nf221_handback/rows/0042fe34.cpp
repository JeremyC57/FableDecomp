#pragma optimize("s", on)
// CCountedPointer<const CFontBank>::operator=<CFontBank>(const CCountedPointer<CFontBank>&) -- the converting member template: ShareData(o.Data, o.Info), return *this
// Leaf: ??$?4VCFontBank@@@?$CCountedPointer@$$CBVCFontBank@@@@QAEAAV0@ABV?$CCountedPointer@VCFontBank@@@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0042FE34, 23 bytes, thiscall `ret 4`. Callee: 0x0042F9F2.
// Identity (NF221, family q12226 / q12402): FableWin.pdb ??$?4VCFontBank@@@?$CCountedPointer@$$CBVCFontBank@@@@QAEAAV0@ABV?$CCountedPointer@VCFontBank@@@@@Z; calls ShareData<const CFontBank> 0x0042F9F2; its caller 0x0042DDB3 assigns CFontManager::GetFont's CCountedPointer<CFontBank> temporary into the CCountedPointer<const CFontBank> member +0x10 and then releases the temporary with ReleaseData<CFontBank> 0x004057A0.
// Was ?AssignFrom@CCPtr@@QAEAAU1@ABU1@@Z (local CCPtr stand-in); ported onto engine/CCountedPointer.h (FABLE_RETAIL_CCOUNTEDPOINTER_SHAREDATA_DECL_ONLY: the callee is out of line in this size TU).
#define FABLE_RETAIL_CCOUNTEDPOINTER_SHAREDATA_DECL_ONLY
#include "engine/CCountedPointer.h"
class CFontBank;
template CCountedPointer<const CFontBank>& CCountedPointer<const CFontBank>::operator=<CFontBank>(const CCountedPointer<CFontBank>&);
