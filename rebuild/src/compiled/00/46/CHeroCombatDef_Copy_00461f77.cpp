// Leaf: ?Copy@CHeroCombatDef@@UAEXPBVCDefClassBase@@@Z
// CHeroCombatDef::Copy(const CDefClassBase* other) -- copy every member of another CHeroCombatDef: `*this = *(const CHeroCombatDef*)other;`
// Retail: Fable.exe (TLC, VC7.1) @ 0x00461F77, 5 bytes: a thiscall tail call, `jmp` to CHeroCombatDef::operator= 0x00461F7C (emitted right after it).
// Identity: the RTTI vftable placement of 0x00461F77 (ghidra_out/labels_rtti_port.tsv, slot 19 Copy) + Ego_r.pdb / FableWin.pdb name sets.
// Callee: CHeroCombatDef::operator=(const CHeroCombatDef&) 0x00461F7C, ??4CHeroCombatDef@@QAEAAV0@ABV0@@Z (PDB name set + the landed operator= row's own definition at the jmp target).
// NF337 (2026-11-24): moved from the flattened __fastcall stand-in and its made-up `_Copy_impl` callee onto the PDB member; operator= is
//   declared out of line here, so the body is the one-instruction tail call retail has.
#define FABLE_RETAIL_FIRE_RANGED_WEAPON
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "rebuild_abi.h"
#include "engine/CHeroCombatDef.h"

void CHeroCombatDef::Copy(const CDefClassBase* other)
{
    *this = *(const CHeroCombatDef*)other;
}
