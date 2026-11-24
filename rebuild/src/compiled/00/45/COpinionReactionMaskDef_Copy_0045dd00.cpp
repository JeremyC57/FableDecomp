// Leaf: ?Copy@COpinionReactionMaskDef@@UAEXPBVCDefClassBase@@@Z
// COpinionReactionMaskDef::Copy(const CDefClassBase* other) -- copy every member of another COpinionReactionMaskDef: `*this = *(const COpinionReactionMaskDef*)other;`
// Retail: Fable.exe (TLC, VC7.1) @ 0x0045DD00, 5 bytes: a thiscall tail call, `jmp` to COpinionReactionMaskDef::operator= 0x0045DD05 (emitted right after it).
// Identity: the RTTI vftable placement of 0x0045DD00 (ghidra_out/labels_rtti_port.tsv, slot 19 Copy) + Ego_r.pdb / FableWin.pdb name sets.
// Callee: COpinionReactionMaskDef::operator=(const COpinionReactionMaskDef&) 0x0045DD05, ??4COpinionReactionMaskDef@@QAEAAV0@ABV0@@Z (PDB name set + the landed operator= row's own definition at the jmp target).
// NF337 (2026-11-24): moved from the flattened __fastcall stand-in and its made-up `_Copy_impl` callee onto the PDB member; operator= is
//   declared out of line here, so the body is the one-instruction tail call retail has.
#include "rebuild_abi.h"

class CDefClassBase;
class COpinionReactionMaskDef {                                // declarations only (no engine/COpinionReactionMaskDef.h)
public:
    virtual void Copy(const CDefClassBase* other);              // vftable slot 19, 0x0045DD00
    COpinionReactionMaskDef& operator=(const COpinionReactionMaskDef& other);   // out of line: 0x0045DD05
};

void COpinionReactionMaskDef::Copy(const CDefClassBase* other)
{
    *this = *(const COpinionReactionMaskDef*)other;
}
