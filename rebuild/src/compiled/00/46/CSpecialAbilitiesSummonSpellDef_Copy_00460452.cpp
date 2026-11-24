// Leaf: ?Copy@CSpecialAbilitiesSummonSpellDef@@UAEXPBVCDefClassBase@@@Z
// CSpecialAbilitiesSummonSpellDef::Copy(const CDefClassBase* other) -- copy every member of another CSpecialAbilitiesSummonSpellDef: `*this = *(const CSpecialAbilitiesSummonSpellDef*)other;`
// Retail: Fable.exe (TLC, VC7.1) @ 0x00460452, 5 bytes: a thiscall tail call, `jmp` to CSpecialAbilitiesSummonSpellDef::operator= 0x00460457 (emitted right after it).
// Identity: the RTTI vftable placement of 0x00460452 (ghidra_out/labels_rtti_port.tsv, slot 19 Copy) + Ego_r.pdb / FableWin.pdb name sets.
// Callee: CSpecialAbilitiesSummonSpellDef::operator=(const CSpecialAbilitiesSummonSpellDef&) 0x00460457, ??4CSpecialAbilitiesSummonSpellDef@@QAEAAV0@ABV0@@Z (PDB name set + the landed operator= row's own definition at the jmp target).
// NF337 (2026-11-24): moved from the flattened __fastcall stand-in and its made-up `_Copy_impl` callee onto the PDB member; operator= is
//   declared out of line here, so the body is the one-instruction tail call retail has.
#include "rebuild_abi.h"

class CDefClassBase;
class CSpecialAbilitiesSummonSpellDef {                                // declarations only (no engine/CSpecialAbilitiesSummonSpellDef.h)
public:
    virtual void Copy(const CDefClassBase* other);              // vftable slot 19, 0x00460452
    CSpecialAbilitiesSummonSpellDef& operator=(const CSpecialAbilitiesSummonSpellDef& other);   // out of line: 0x00460457
};

void CSpecialAbilitiesSummonSpellDef::Copy(const CDefClassBase* other)
{
    *this = *(const CSpecialAbilitiesSummonSpellDef*)other;
}
