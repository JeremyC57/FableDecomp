// Leaf: ?Copy@CSpecialAbilitiesBattleChargeDef@@UAEXPBVCDefClassBase@@@Z
// CSpecialAbilitiesBattleChargeDef::Copy(const CDefClassBase* other) -- copy every member of another CSpecialAbilitiesBattleChargeDef: `*this = *(const CSpecialAbilitiesBattleChargeDef*)other;`
// Retail: Fable.exe (TLC, VC7.1) @ 0x0045F630, 5 bytes: a thiscall tail call, `jmp` to CSpecialAbilitiesBattleChargeDef::operator= 0x0045F635 (emitted right after it).
// Identity: the RTTI vftable placement of 0x0045F630 (ghidra_out/labels_rtti_port.tsv, slot 19 Copy) + Ego_r.pdb / FableWin.pdb name sets.
// Callee: CSpecialAbilitiesBattleChargeDef::operator=(const CSpecialAbilitiesBattleChargeDef&) 0x0045F635, ??4CSpecialAbilitiesBattleChargeDef@@QAEAAV0@ABV0@@Z (PDB name set + the landed operator= row's own definition at the jmp target).
// NF337 (2026-11-24): moved from the flattened __fastcall stand-in and its made-up `_Copy_impl` callee onto the PDB member; operator= is
//   declared out of line here, so the body is the one-instruction tail call retail has.
#include "rebuild_abi.h"

class CDefClassBase;
class CSpecialAbilitiesBattleChargeDef {                                // declarations only (no engine/CSpecialAbilitiesBattleChargeDef.h)
public:
    virtual void Copy(const CDefClassBase* other);              // vftable slot 19, 0x0045F630
    CSpecialAbilitiesBattleChargeDef& operator=(const CSpecialAbilitiesBattleChargeDef& other);   // out of line: 0x0045F635
};

void CSpecialAbilitiesBattleChargeDef::Copy(const CDefClassBase* other)
{
    *this = *(const CSpecialAbilitiesBattleChargeDef*)other;
}
