#pragma optimize("s",on)
// CCountedPointer<CVillageTaskTorch>::ReleaseData() -- release one counted pointer (the destructor is a jmp thunk to it)
// Leaf: ?ReleaseData@?$CCountedPointer@VCVillageTaskTorch@@@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0065D574, 44 bytes. Callees: the count block's DeleteFunc (indirect), operator delete 0x00BFE9BC.
//
// The shared count block's RefCount drops by one; at zero its DeleteFunc runs on the block's Data and the block is
// freed. Data and then Info are cleared (the destructor's Data-first order, engine/CCountedPointer.h: retail emits one
// ~CCountedPointer COMDAT per T). The catalog's "Release" placeholder and the earlier _Dest_val label name this body; the
// order of the two stores is the destructor's, not ReleaseData's (Info first).
// T = CVillageTaskTorch: caller binding CVillageTaskTorch (00660117) + PDB fablewin ??1 symbol.
// Matching: `this` in ecx; DeleteFunc is __fastcall (Data in ecx); the block goes back through operator delete.
// Name fix (NF202, 2026-11-20, q12230): the 44-byte release at 0x0065D574 is CCountedPointer<CVillageTaskTorch>::ReleaseData (FableWin.pdb
//   ?ReleaseData@?$CCountedPointer@VCVillageTaskTorch@@@@QAEXXZ + the body: the destructor is the 5-byte `jmp` COMDAT(s) 0x0065E298 / 0x0065F925 at /Os,
//   `~CCountedPointer() { ReleaseData(); }`, as engine/CCountedPointer.h documents for CComponent / CAction / CMeshDataBank);
//   FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA (Data zeroed before Info).
#define FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA
#include "engine/CCountedPointer.h"
class CVillageTaskTorch;
template void CCountedPointer<CVillageTaskTorch>::ReleaseData();
