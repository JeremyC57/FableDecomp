// Leaf: ?Copy@CSpecialAbilitiesBurntEffectDef@@UAEXPBVCDefClassBase@@@Z
// CSpecialAbilitiesBurntEffectDef::Copy(const CDefClassBase* other) -- copy every member of another CSpecialAbilitiesBurntEffectDef: `*this = *(const CSpecialAbilitiesBurntEffectDef*)other;`
// Retail: Fable.exe (TLC, VC7.1) @ 0x004604CA, 5 bytes: a thiscall tail call, `jmp` to CSpecialAbilitiesBurntEffectDef::operator= 0x004604CF (emitted right after it).
// Identity: the RTTI vftable placement of 0x004604CA (ghidra_out/labels_rtti_port.tsv, slot 19 Copy) + Ego_r.pdb / FableWin.pdb name sets.
// Callee: CSpecialAbilitiesBurntEffectDef::operator=(const CSpecialAbilitiesBurntEffectDef&) 0x004604CF, ??4CSpecialAbilitiesBurntEffectDef@@QAEAAV0@ABV0@@Z (PDB name set + the landed operator= row's own definition at the jmp target).
// NF337 (2026-11-24): moved from the flattened __fastcall stand-in and its made-up `_Copy_impl` callee onto the PDB member; operator= is
//   declared out of line here, so the body is the one-instruction tail call retail has.
#include "rebuild_abi.h"

class CDefClassBase;
class CSpecialAbilitiesBurntEffectDef {                                // declarations only (no engine/CSpecialAbilitiesBurntEffectDef.h)
public:
    virtual void Copy(const CDefClassBase* other);              // vftable slot 19, 0x004604CA
    CSpecialAbilitiesBurntEffectDef& operator=(const CSpecialAbilitiesBurntEffectDef& other);   // out of line: 0x004604CF
};

void CSpecialAbilitiesBurntEffectDef::Copy(const CDefClassBase* other)
{
    *this = *(const CSpecialAbilitiesBurntEffectDef*)other;
}
