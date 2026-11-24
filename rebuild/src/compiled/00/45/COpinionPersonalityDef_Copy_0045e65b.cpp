// Leaf: ?Copy@COpinionPersonalityDef@@UAEXPBVCDefClassBase@@@Z
// COpinionPersonalityDef::Copy(const CDefClassBase* other) -- copy every member of another COpinionPersonalityDef: `*this = *(const COpinionPersonalityDef*)other;`
// Retail: Fable.exe (TLC, VC7.1) @ 0x0045E65B, 5 bytes: a thiscall tail call, `jmp` to COpinionPersonalityDef::operator= 0x0045E660 (emitted right after it).
// Identity: the RTTI vftable placement of 0x0045E65B (ghidra_out/labels_rtti_port.tsv, slot 19 Copy) + Ego_r.pdb / FableWin.pdb name sets.
// Callee: COpinionPersonalityDef::operator=(const COpinionPersonalityDef&) 0x0045E660, ??4COpinionPersonalityDef@@QAEABV0@ABV0@@Z (Ego_r.pdb + FableWin.pdb, the only ??4 spelling of the class in either; the operator= row respells onto it from the non-const return in this unit).
// NF337 (2026-11-24): moved from the flattened __fastcall stand-in and its made-up `_Copy_impl` callee onto the PDB member; operator= is
//   declared out of line here, so the body is the one-instruction tail call retail has.
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_COPINIONPERSONALITYDEF_COPY_DECLS
#include "rebuild_abi.h"
#include "engine/COpinionPersonalityDef.h"

void COpinionPersonalityDef::Copy(const CDefClassBase* other)
{
    *this = *(const COpinionPersonalityDef*)other;
}
