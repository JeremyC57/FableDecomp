#pragma optimize("s",on)
// CCountedPointer<CVillageTaskSecuritySweep>::ReleaseData() -- release one counted pointer (the destructor is a jmp thunk to it)
// Leaf: ?ReleaseData@?$CCountedPointer@VCVillageTaskSecuritySweep@@@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0065D5CC, 44 bytes. Callees: the count block's DeleteFunc (indirect), operator delete 0x00BFE9BC.
//
// The shared count block's RefCount drops by one; at zero its DeleteFunc runs on the block's Data and the block is
// freed. Data and then Info are cleared (the destructor's Data-first order, engine/CCountedPointer.h: retail emits one
// ~CCountedPointer COMDAT per T). The catalog's "Release" placeholder and the earlier _Dest_val label name this body; the
// order of the two stores is the destructor's, not ReleaseData's (Info first).
// T = CVillageTaskSecuritySweep: catalog T CVillageTaskSecuritySweep + caller binding (006601bf) + PDB fablewin ??1 symbol.
// Matching: `this` in ecx; DeleteFunc is __fastcall (Data in ecx); the block goes back through operator delete.
// Name fix (NF202, 2026-11-20, q12231): the 44-byte release at 0x0065D5CC is CCountedPointer<CVillageTaskSecuritySweep>::ReleaseData (FableWin.pdb
//   ?ReleaseData@?$CCountedPointer@VCVillageTaskSecuritySweep@@@@QAEXXZ + the body: the destructor is the 5-byte `jmp` COMDAT(s) 0x0065E31E / 0x0065F939 at /Os,
//   `~CCountedPointer() { ReleaseData(); }`, as engine/CCountedPointer.h documents for CComponent / CAction / CMeshDataBank);
//   FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA (Data zeroed before Info).
#define FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA
#include "engine/CCountedPointer.h"
class CVillageTaskSecuritySweep;
template void CCountedPointer<CVillageTaskSecuritySweep>::ReleaseData();
