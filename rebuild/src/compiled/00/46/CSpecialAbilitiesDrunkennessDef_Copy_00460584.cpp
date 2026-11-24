// Leaf: ?Copy@CSpecialAbilitiesDrunkennessDef@@UAEXPBVCDefClassBase@@@Z
// CSpecialAbilitiesDrunkennessDef::Copy(const CDefClassBase* other) -- copy every member of another CSpecialAbilitiesDrunkennessDef: `*this = *(const CSpecialAbilitiesDrunkennessDef*)other;`
// Retail: Fable.exe (TLC, VC7.1) @ 0x00460584, 5 bytes: a thiscall tail call, `jmp` to CSpecialAbilitiesDrunkennessDef::operator= 0x00460589 (emitted right after it).
// Identity: the RTTI vftable placement of 0x00460584 (ghidra_out/labels_rtti_port.tsv, slot 19 Copy) + Ego_r.pdb / FableWin.pdb name sets.
// Callee: CSpecialAbilitiesDrunkennessDef::operator=(const CSpecialAbilitiesDrunkennessDef&) 0x00460589, ??4CSpecialAbilitiesDrunkennessDef@@QAEAAV0@ABV0@@Z (Ego_r.pdb + FableWin.pdb; the operator= row respells onto it from the made-up CBase_00431ed0_Derived_00460589 in this unit).
// NF337 (2026-11-24): moved from the flattened __fastcall stand-in and its made-up `_Copy_impl` callee onto the PDB member; operator= is
//   declared out of line here, so the body is the one-instruction tail call retail has.
#include "rebuild_abi.h"

class CDefClassBase;
class CSpecialAbilitiesDrunkennessDef {                                // declarations only (no engine/CSpecialAbilitiesDrunkennessDef.h)
public:
    virtual void Copy(const CDefClassBase* other);              // vftable slot 19, 0x00460584
    CSpecialAbilitiesDrunkennessDef& operator=(const CSpecialAbilitiesDrunkennessDef& other);   // out of line: 0x00460589
};

void CSpecialAbilitiesDrunkennessDef::Copy(const CDefClassBase* other)
{
    *this = *(const CSpecialAbilitiesDrunkennessDef*)other;
}
