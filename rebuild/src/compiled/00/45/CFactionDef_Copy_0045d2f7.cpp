// Leaf: ?Copy@CFactionDef@@UAEXPBVCDefClassBase@@@Z
// CFactionDef::Copy(const CDefClassBase* other) -- copy every member of another CFactionDef: `*this = *(const CFactionDef*)other;`
// Retail: Fable.exe (TLC, VC7.1) @ 0x0045D2F7, 5 bytes: a thiscall tail call, `jmp` to CFactionDef::operator= 0x0045D2FC (emitted right after it).
// Identity: the RTTI vftable placement of 0x0045D2F7 (ghidra_out/labels_rtti_port.tsv, slot 19 Copy) + Ego_r.pdb / FableWin.pdb name sets.
// Callee: CFactionDef::operator=(const CFactionDef&) 0x0045D2FC, ??4CFactionDef@@QAEAAV0@ABV0@@Z (PDB name set + the landed operator= row's own definition at the jmp target).
// NF337 (2026-11-24): moved from the flattened __fastcall stand-in and its made-up `_Copy_impl` callee onto the PDB member; operator= is
//   declared out of line here, so the body is the one-instruction tail call retail has.
#include "rebuild_abi.h"

class CDefClassBase;
class CFactionDef {                                // declarations only (no engine/CFactionDef.h)
public:
    virtual void Copy(const CDefClassBase* other);              // vftable slot 19, 0x0045D2F7
    CFactionDef& operator=(const CFactionDef& other);   // out of line: 0x0045D2FC
};

void CFactionDef::Copy(const CDefClassBase* other)
{
    *this = *(const CFactionDef*)other;
}
