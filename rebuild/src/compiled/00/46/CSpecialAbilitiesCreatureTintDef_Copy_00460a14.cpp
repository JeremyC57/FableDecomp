// Leaf: ?Copy@CSpecialAbilitiesCreatureTintDef@@UAEXPBVCDefClassBase@@@Z
// CSpecialAbilitiesCreatureTintDef::Copy(const CDefClassBase* other) -- copy every member of another CSpecialAbilitiesCreatureTintDef: `*this = *(const CSpecialAbilitiesCreatureTintDef*)other;`
// Retail: Fable.exe (TLC, VC7.1) @ 0x00460A14, 5 bytes: a thiscall tail call, `jmp` to CSpecialAbilitiesCreatureTintDef::operator= 0x00460A19 (emitted right after it).
// Identity: the RTTI vftable placement of 0x00460A14 (ghidra_out/labels_rtti_port.tsv, slot 19 Copy) + Ego_r.pdb / FableWin.pdb name sets.
// Callee: CSpecialAbilitiesCreatureTintDef::operator=(const CSpecialAbilitiesCreatureTintDef&) 0x00460A19, ??4CSpecialAbilitiesCreatureTintDef@@QAEAAV0@ABV0@@Z (PDB name set + the landed operator= row's own definition at the jmp target).
// NF386 (2026-11-24; its T3_PORT_QUEUE line: a flatten / alias skip): moved from the flattened __fastcall stand-in and its made-up callee onto the PDB member; operator= is
//   declared out of line here, so the body is the one-instruction tail call retail has.
#include "rebuild_abi.h"

class CDefClassBase;
class CSpecialAbilitiesCreatureTintDef {                                // declarations only (no engine/CSpecialAbilitiesCreatureTintDef.h)
public:
    virtual void Copy(const CDefClassBase* other);              // vftable slot 19, 0x00460A14
    CSpecialAbilitiesCreatureTintDef& operator=(const CSpecialAbilitiesCreatureTintDef& other);   // out of line: 0x00460A19
};

void CSpecialAbilitiesCreatureTintDef::Copy(const CDefClassBase* other)
{
    *this = *(const CSpecialAbilitiesCreatureTintDef*)other;
}
