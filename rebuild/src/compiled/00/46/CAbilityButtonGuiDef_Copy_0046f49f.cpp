// Leaf: ?Copy@CAbilityButtonGuiDef@@UAEXPBVCDefClassBase@@@Z
// CAbilityButtonGuiDef::Copy(const CDefClassBase* other) -- copy every member of another CAbilityButtonGuiDef: `*this = *(const CAbilityButtonGuiDef*)other;`
// Retail: Fable.exe (TLC, VC7.1) @ 0x0046F49F, 5 bytes: a thiscall tail call, `jmp` to CAbilityButtonGuiDef::operator= 0x0046F43E (emitted 0x61 bytes before it).
// Identity: the RTTI vftable placement of 0x0046F49F (ghidra_out/labels_rtti_port.tsv, slot 19 Copy) + Ego_r.pdb / FableWin.pdb name sets.
// Callee: CAbilityButtonGuiDef::operator=(const CAbilityButtonGuiDef&) 0x0046F43E, ??4CAbilityButtonGuiDef@@QAEAAV0@ABV0@@Z (Ego_r.pdb + FableWin.pdb; the landed operator= row 0x0046F43E defines it, which in this unit stops defining this Copy).
// NF337 (2026-11-24): moved from the flattened __fastcall stand-in and its made-up `_Copy_impl` callee onto the PDB member; operator= is
//   declared out of line here, so the body is the one-instruction tail call retail has.
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CABILITYBUTTONGUIDEF_COPY_DECLS
#include "rebuild_abi.h"
#include "engine/CAbilityButtonGuiDef.h"

void CAbilityButtonGuiDef::Copy(const CDefClassBase* other)
{
    *this = *(const CAbilityButtonGuiDef*)other;
}
