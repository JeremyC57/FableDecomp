// Leaf: ?Copy@CSpecialAbilitiesGhostSwordDef@@UAEXPBVCDefClassBase@@@Z
// CSpecialAbilitiesGhostSwordDef::Copy(const CDefClassBase* other) -- copy every member of another CSpecialAbilitiesGhostSwordDef: `*this = *(const CSpecialAbilitiesGhostSwordDef*)other;`
// Retail: Fable.exe (TLC, VC7.1) @ 0x00460180, 5 bytes: a thiscall tail call, `jmp` to CSpecialAbilitiesGhostSwordDef::operator= 0x00460185 (emitted right after it).
// Identity: the RTTI vftable placement of 0x00460180 (ghidra_out/labels_rtti_port.tsv, slot 19 Copy) + Ego_r.pdb / FableWin.pdb name sets.
// Callee: CSpecialAbilitiesGhostSwordDef::operator=(const CSpecialAbilitiesGhostSwordDef&) 0x00460185, ??4CSpecialAbilitiesGhostSwordDef@@QAEAAV0@ABV0@@Z (PDB name set + the landed operator= row's own definition at the jmp target).
// NF337 (2026-11-24): moved from the flattened __fastcall stand-in and its made-up `_Copy_impl` callee onto the PDB member; operator= is
//   declared out of line here, so the body is the one-instruction tail call retail has.
#include "rebuild_abi.h"

class CDefClassBase;
class CSpecialAbilitiesGhostSwordDef {                                // declarations only (no engine/CSpecialAbilitiesGhostSwordDef.h)
public:
    virtual void Copy(const CDefClassBase* other);              // vftable slot 19, 0x00460180
    CSpecialAbilitiesGhostSwordDef& operator=(const CSpecialAbilitiesGhostSwordDef& other);   // out of line: 0x00460185
};

void CSpecialAbilitiesGhostSwordDef::Copy(const CDefClassBase* other)
{
    *this = *(const CSpecialAbilitiesGhostSwordDef*)other;
}
