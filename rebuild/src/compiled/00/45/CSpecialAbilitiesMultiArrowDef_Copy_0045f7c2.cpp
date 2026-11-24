// Leaf: ?Copy@CSpecialAbilitiesMultiArrowDef@@UAEXPBVCDefClassBase@@@Z
// CSpecialAbilitiesMultiArrowDef::Copy(const CDefClassBase* other) -- copy every member of another CSpecialAbilitiesMultiArrowDef: `*this = *(const CSpecialAbilitiesMultiArrowDef*)other;`
// Retail: Fable.exe (TLC, VC7.1) @ 0x0045F7C2, 5 bytes: a thiscall tail call, `jmp` to CSpecialAbilitiesMultiArrowDef::operator= 0x0045F7C7 (emitted right after it).
// Identity: the RTTI vftable placement of 0x0045F7C2 (ghidra_out/labels_rtti_port.tsv, slot 19 Copy) + Ego_r.pdb / FableWin.pdb name sets.
// Callee: CSpecialAbilitiesMultiArrowDef::operator=(const CSpecialAbilitiesMultiArrowDef&) 0x0045F7C7, ??4CSpecialAbilitiesMultiArrowDef@@QAEAAV0@ABV0@@Z (PDB name set + the landed operator= row's own definition at the jmp target).
// NF337 (2026-11-24): moved from the flattened __fastcall stand-in and its made-up `_Copy_impl` callee onto the PDB member; operator= is
//   declared out of line here, so the body is the one-instruction tail call retail has.
#include "rebuild_abi.h"

class CDefClassBase;
class CSpecialAbilitiesMultiArrowDef {                                // declarations only (no engine/CSpecialAbilitiesMultiArrowDef.h)
public:
    virtual void Copy(const CDefClassBase* other);              // vftable slot 19, 0x0045F7C2
    CSpecialAbilitiesMultiArrowDef& operator=(const CSpecialAbilitiesMultiArrowDef& other);   // out of line: 0x0045F7C7
};

void CSpecialAbilitiesMultiArrowDef::Copy(const CDefClassBase* other)
{
    *this = *(const CSpecialAbilitiesMultiArrowDef*)other;
}
