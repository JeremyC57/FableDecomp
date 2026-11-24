// Leaf: ?Copy@CCombatDialogueDef@@UAEXPBVCDefClassBase@@@Z
// CCombatDialogueDef::Copy(const CDefClassBase* other) -- copy every member of another CCombatDialogueDef: `*this = *(const CCombatDialogueDef*)other;`
// Retail: Fable.exe (TLC, VC7.1) @ 0x0045E555, 5 bytes: a thiscall tail call, `jmp` to CCombatDialogueDef::operator= 0x0045E55A (emitted right after it).
// Identity: the RTTI vftable placement of 0x0045E555 (ghidra_out/labels_rtti_port.tsv, slot 19 Copy) + Ego_r.pdb / FableWin.pdb name sets.
// Callee: CCombatDialogueDef::operator=(const CCombatDialogueDef&) 0x0045E55A, ??4CCombatDialogueDef@@QAEAAV0@ABV0@@Z (PDB name set + the landed operator= row's own definition at the jmp target).
// NF337 (2026-11-24): moved from the flattened __fastcall stand-in and its made-up `_Copy_impl` callee onto the PDB member; operator= is
//   declared out of line here, so the body is the one-instruction tail call retail has.
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "rebuild_abi.h"
#include "engine/CCombatDialogueDef.h"

void CCombatDialogueDef::Copy(const CDefClassBase* other)
{
    *this = *(const CCombatDialogueDef*)other;
}
