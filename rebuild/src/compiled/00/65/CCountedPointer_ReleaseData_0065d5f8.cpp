#pragma optimize("s",on)
// CCountedPointer<CVillageTaskCarryCrate>::ReleaseData() -- release one counted pointer (the destructor is a jmp thunk to it)
// Leaf: ?ReleaseData@?$CCountedPointer@VCVillageTaskCarryCrate@@@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0065D5F8, 44 bytes. Callees: the count block's DeleteFunc (indirect), operator delete 0x00BFE9BC.
//
// The shared count block's RefCount drops by one; at zero its DeleteFunc runs on the block's Data and the block is
// freed. Data and then Info are cleared (the destructor's Data-first order, engine/CCountedPointer.h: retail emits one
// ~CCountedPointer COMDAT per T). The catalog's "Release" placeholder and the earlier _Dest_val label name this body; the
// order of the two stores is the destructor's, not ReleaseData's (Info first).
// T = CVillageTaskCarryCrate: caller binding CVillageTaskCarryCrate (00659c7e,006601e9) + PDB fablewin ??1 symbol.
// Matching: `this` in ecx; DeleteFunc is __fastcall (Data in ecx); the block goes back through operator delete.
// Name fix (NF202, 2026-11-20, q12232): the 44-byte release at 0x0065D5F8 is CCountedPointer<CVillageTaskCarryCrate>::ReleaseData (FableWin.pdb
//   ?ReleaseData@?$CCountedPointer@VCVillageTaskCarryCrate@@@@QAEXXZ + the body: the destructor is the 5-byte `jmp` COMDAT(s) 0x0065E379 / 0x0065F93E at /Os,
//   `~CCountedPointer() { ReleaseData(); }`, as engine/CCountedPointer.h documents for CComponent / CAction / CMeshDataBank);
//   FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA (Data zeroed before Info).
#define FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA
#include "engine/CCountedPointer.h"
class CVillageTaskCarryCrate;
template void CCountedPointer<CVillageTaskCarryCrate>::ReleaseData();
