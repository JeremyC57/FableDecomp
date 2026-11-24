// Leaf: ?Copy@CSpecialAbilitiesElectrocutedEffectDef@@UAEXPBVCDefClassBase@@@Z
// CSpecialAbilitiesElectrocutedEffectDef::Copy(const CDefClassBase* other) -- copy every member of another CSpecialAbilitiesElectrocutedEffectDef: `*this = *(const CSpecialAbilitiesElectrocutedEffectDef*)other;`
// Retail: Fable.exe (TLC, VC7.1) @ 0x00460518, 5 bytes: a thiscall tail call, `jmp` to CSpecialAbilitiesElectrocutedEffectDef::operator= 0x0046051D (emitted right after it).
// Identity: the RTTI vftable placement of 0x00460518 (ghidra_out/labels_rtti_port.tsv, slot 19 Copy) + Ego_r.pdb / FableWin.pdb name sets.
// Callee: CSpecialAbilitiesElectrocutedEffectDef::operator=(const CSpecialAbilitiesElectrocutedEffectDef&) 0x0046051D, ??4CSpecialAbilitiesElectrocutedEffectDef@@QAEAAV0@ABV0@@Z (PDB name set + the landed operator= row's own definition at the jmp target).
// NF337 (2026-11-24): moved from the flattened __fastcall stand-in and its made-up `_Copy_impl` callee onto the PDB member; operator= is
//   declared out of line here, so the body is the one-instruction tail call retail has.
#include "rebuild_abi.h"

class CDefClassBase;
class CSpecialAbilitiesElectrocutedEffectDef {                                // declarations only (no engine/CSpecialAbilitiesElectrocutedEffectDef.h)
public:
    virtual void Copy(const CDefClassBase* other);              // vftable slot 19, 0x00460518
    CSpecialAbilitiesElectrocutedEffectDef& operator=(const CSpecialAbilitiesElectrocutedEffectDef& other);   // out of line: 0x0046051D
};

void CSpecialAbilitiesElectrocutedEffectDef::Copy(const CDefClassBase* other)
{
    *this = *(const CSpecialAbilitiesElectrocutedEffectDef*)other;
}
