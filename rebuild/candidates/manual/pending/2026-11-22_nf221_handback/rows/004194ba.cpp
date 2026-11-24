#pragma optimize("s", on)
// CCountedPointer<CGraphicDataBank>::operator=(const CCountedPointer&) -- ShareData(o.Data, o.Info), return *this
// Leaf: ??4?$CCountedPointer@VCGraphicDataBank@@@@QAEAAV0@ABV0@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x004194BA, 23 bytes, thiscall `ret 4`. Callee: 0x00419134.
// Identity (NF221, family q12226 / q12402): FableWin.pdb ??4?$CCountedPointer@VCGraphicDataBank@@@@QAEAAV0@ABV0@@Z; loads o.Data / o.Info and calls ShareData<CGraphicDataBank> 0x00419134 out of line; landed caller 0x00416C8A already binds this symbol.
// Was ?AssignFrom@CCPtr@@QAEAAU1@ABU1@@Z (local CCPtr stand-in); ported onto engine/CCountedPointer.h (FABLE_RETAIL_CCOUNTEDPOINTER_SHAREDATA_DECL_ONLY: the callee is out of line in this size TU).
#define FABLE_RETAIL_CCOUNTEDPOINTER_SHAREDATA_DECL_ONLY
#include "engine/CCountedPointer.h"
class CGraphicDataBank;
template CCountedPointer<CGraphicDataBank>& CCountedPointer<CGraphicDataBank>::operator=(const CCountedPointer<CGraphicDataBank>&);
