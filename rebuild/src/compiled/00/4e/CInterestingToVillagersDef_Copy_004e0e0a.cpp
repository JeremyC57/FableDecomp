// Leaf: ?Copy@CInterestingToVillagersDef@@UAEXPBVCDefClassBase@@@Z
// CInterestingToVillagersDef::Copy(const CDefClassBase* other) -- copy another CInterestingToVillagersDef: the CDefClassBase part through its out-of-line operator=, then Interest
// Retail: Fable.exe (TLC, VC7.1) @ 0x004E0E0A, thiscall, `ret 4`: `call 0x00431F10` (CDefClassBase::operator=, this = ecx, other pushed), then the
//   dword Interest +0x28 from other to this (the derived assignment is inline in retail: no out-of-line CInterestingToVillagersDef::operator= is called).
// Identity: the RTTI vftable placement of 0x004E0E0A (ghidra_out/labels_rtti_port.tsv, slot 19 Copy) + Ego_r.pdb / FableWin.pdb name sets.
// Callee: CDefClassBase::operator=(const CDefClassBase&) 0x00431F10, ??4CDefClassBase@@QAEAAV0@ABV0@@Z (Ego_r.pdb + FableWin.pdb; engine/CDefClassBase.h).
// NF337 (2026-11-24): moved from the flattened __fastcall stand-in onto the PDB member on the shared class model (engine/CInterestingToVillagersDef.h FABLE_RETAIL_FIND_INTERESTING, which declares Copy; no header edit); the explicit
//   base call + member copy is the inline assignment's expansion: the header declares the PDB's out-of-line CInterestingToVillagersDef::operator=, so `*this = ...` would call it (retail inlines it).
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_FIND_INTERESTING
#include "rebuild_abi.h"
#include "engine/CInterestingToVillagersDef.h"

void CInterestingToVillagersDef::Copy(const CDefClassBase* other)
{
    CDefClassBase::operator=(*other);
    Interest = ((const CInterestingToVillagersDef*)other)->Interest;
}
