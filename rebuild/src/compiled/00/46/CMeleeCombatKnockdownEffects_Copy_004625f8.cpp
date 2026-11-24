// Leaf: ?Copy@CMeleeCombatKnockdownEffects@@UAEXPBVCDefClassBase@@@Z
// CMeleeCombatKnockdownEffects::Copy(const CDefClassBase* other) -- copy every member of another CMeleeCombatKnockdownEffects: `*this = *(const CMeleeCombatKnockdownEffects*)other;`
// Retail: Fable.exe (TLC, VC7.1) @ 0x004625F8, 5 bytes: a thiscall tail call, `jmp` to CMeleeCombatKnockdownEffects::operator= 0x004625FD (emitted right after it).
// Identity: the RTTI vftable placement of 0x004625F8 (ghidra_out/labels_rtti_port.tsv, slot 19 Copy) + Ego_r.pdb / FableWin.pdb name sets.
// Callee: CMeleeCombatKnockdownEffects::operator=(const CMeleeCombatKnockdownEffects&) 0x004625FD, ??4CMeleeCombatKnockdownEffects@@QAEAAV0@ABV0@@Z (PDB name set + the landed operator= row's own definition at the jmp target).
// NF337 (2026-11-24): moved from the flattened __fastcall stand-in and its made-up `_Copy_impl` callee onto the PDB member; operator= is
//   declared out of line here, so the body is the one-instruction tail call retail has.
#include "rebuild_abi.h"

class CDefClassBase;
class CMeleeCombatKnockdownEffects {                                // declarations only (no engine/CMeleeCombatKnockdownEffects.h)
public:
    virtual void Copy(const CDefClassBase* other);              // vftable slot 19, 0x004625F8
    CMeleeCombatKnockdownEffects& operator=(const CMeleeCombatKnockdownEffects& other);   // out of line: 0x004625FD
};

void CMeleeCombatKnockdownEffects::Copy(const CDefClassBase* other)
{
    *this = *(const CMeleeCombatKnockdownEffects*)other;
}
