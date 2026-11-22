#pragma optimize("s", on)
// CCountedPointer<CFontBank>::operator=(const CCountedPointer&) -- ShareData(o.Data, o.Info), return *this
// Leaf: ??4?$CCountedPointer@VCFontBank@@@@QAEAAV0@ABV0@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00419463, 23 bytes, thiscall `ret 4`. Callee: 0x004190E2.
// Identity (NF221, family q12226 / q12402): FableWin.pdb ??4?$CCountedPointer@VCFontBank@@@@QAEAAV0@ABV0@@Z; calls ShareData<CFontBank> 0x004190E2 out of line; landed caller 0x00499AA0 already binds this symbol.
// Was ?AssignFrom@CCPtr@@QAEAAU1@ABU1@@Z (local CCPtr stand-in); ported onto engine/CCountedPointer.h (FABLE_RETAIL_CCOUNTEDPOINTER_SHAREDATA_DECL_ONLY: the callee is out of line in this size TU).
#define FABLE_RETAIL_CCOUNTEDPOINTER_SHAREDATA_DECL_ONLY
#include "engine/CCountedPointer.h"
class CFontBank;
template CCountedPointer<CFontBank>& CCountedPointer<CFontBank>::operator=(const CCountedPointer<CFontBank>&);
