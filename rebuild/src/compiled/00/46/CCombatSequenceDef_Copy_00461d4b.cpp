// Leaf: ?Copy@CCombatSequenceDef@@UAEXPBVCDefClassBase@@@Z
// CCombatSequenceDef::Copy(const CDefClassBase* other) -- copy every member of another CCombatSequenceDef: `*this = *(const CCombatSequenceDef*)other;`
// Retail: Fable.exe (TLC, VC7.1) @ 0x00461D4B, 5 bytes: a thiscall tail call, `jmp` to CCombatSequenceDef::operator= 0x00461D50 (emitted right after it).
// Identity: the RTTI vftable placement of 0x00461D4B (ghidra_out/labels_rtti_port.tsv, slot 19 Copy) + Ego_r.pdb / FableWin.pdb name sets.
// Callee: CCombatSequenceDef::operator=(const CCombatSequenceDef&) 0x00461D50, ??4CCombatSequenceDef@@QAEAAV0@ABV0@@Z (PDB name set + the landed operator= row's own definition at the jmp target).
// NF337 (2026-11-24): moved from the flattened __fastcall stand-in and its made-up `_Copy_impl` callee onto the PDB member; operator= is
//   declared out of line here, so the body is the one-instruction tail call retail has.
#include "rebuild_abi.h"

class CDefClassBase;
class CCombatSequenceDef {                                // declarations only (no engine/CCombatSequenceDef.h)
public:
    virtual void Copy(const CDefClassBase* other);              // vftable slot 19, 0x00461D4B
    CCombatSequenceDef& operator=(const CCombatSequenceDef& other);   // out of line: 0x00461D50
};

void CCombatSequenceDef::Copy(const CDefClassBase* other)
{
    *this = *(const CCombatSequenceDef*)other;
}
