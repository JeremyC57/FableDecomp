// Leaf: ?Copy@COpinionDeedMaskDef@@UAEXPBVCDefClassBase@@@Z
// COpinionDeedMaskDef::Copy(const CDefClassBase* other) -- copy every member of another COpinionDeedMaskDef: `*this = *(const COpinionDeedMaskDef*)other;`
// Retail: Fable.exe (TLC, VC7.1) @ 0x0045E182, 5 bytes: a thiscall tail call, `jmp` to COpinionDeedMaskDef::operator= 0x0045E187 (emitted right after it).
// Identity: the RTTI vftable placement of 0x0045E182 (ghidra_out/labels_rtti_port.tsv, slot 19 Copy) + Ego_r.pdb / FableWin.pdb name sets.
// Callee: COpinionDeedMaskDef::operator=(const COpinionDeedMaskDef&) 0x0045E187, ??4COpinionDeedMaskDef@@QAEAAV0@ABV0@@Z (PDB name set + the landed operator= row's own definition at the jmp target).
// NF337 (2026-11-24): moved from the flattened __fastcall stand-in and its made-up `_Copy_impl` callee onto the PDB member; operator= is
//   declared out of line here, so the body is the one-instruction tail call retail has.
#include "rebuild_abi.h"

class CDefClassBase;
class COpinionDeedMaskDef {                                // declarations only (no engine/COpinionDeedMaskDef.h)
public:
    virtual void Copy(const CDefClassBase* other);              // vftable slot 19, 0x0045E182
    COpinionDeedMaskDef& operator=(const COpinionDeedMaskDef& other);   // out of line: 0x0045E187
};

void COpinionDeedMaskDef::Copy(const CDefClassBase* other)
{
    *this = *(const COpinionDeedMaskDef*)other;
}
