// Leaf: ?Copy@COpinionDeedEffectsDef@@UAEXPBVCDefClassBase@@@Z
// COpinionDeedEffectsDef::Copy(const CDefClassBase* other) -- copy every member of another COpinionDeedEffectsDef: `*this = *(const COpinionDeedEffectsDef*)other;`
// Retail: Fable.exe (TLC, VC7.1) @ 0x00471A72, 5 bytes: a thiscall tail call, `jmp` to COpinionDeedEffectsDef::operator= 0x00471A77 (emitted right after it).
// Identity: the RTTI vftable placement of 0x00471A72 (ghidra_out/labels_rtti_port.tsv, slot 19 Copy) + Ego_r.pdb / FableWin.pdb name sets.
// Callee: COpinionDeedEffectsDef::operator=(const COpinionDeedEffectsDef&) 0x00471A77, ??4COpinionDeedEffectsDef@@QAEAAV0@ABV0@@Z (PDB name set + the landed operator= row's own definition at the jmp target).
// NF337 (2026-11-24): moved from the flattened __fastcall stand-in and its made-up `_Copy_impl` callee onto the PDB member; operator= is
//   declared out of line here, so the body is the one-instruction tail call retail has.
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "rebuild_abi.h"
#include "engine/COpinionDeedEffectsDef.h"

void COpinionDeedEffectsDef::Copy(const CDefClassBase* other)
{
    *this = *(const COpinionDeedEffectsDef*)other;
}
