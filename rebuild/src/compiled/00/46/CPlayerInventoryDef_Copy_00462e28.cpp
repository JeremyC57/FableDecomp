// Leaf: ?Copy@CPlayerInventoryDef@@UAEXPBVCDefClassBase@@@Z
// CPlayerInventoryDef::Copy(const CDefClassBase* other) -- copy every member of another CPlayerInventoryDef: `*this = *(const CPlayerInventoryDef*)other;`
// Retail: Fable.exe (TLC, VC7.1) @ 0x00462E28, 5 bytes: a thiscall tail call, `jmp` to CPlayerInventoryDef::operator= 0x00462E2D (emitted right after it).
// Identity: the RTTI vftable placement of 0x00462E28 (ghidra_out/labels_rtti_port.tsv, slot 19 Copy) + Ego_r.pdb / FableWin.pdb name sets.
// Callee: CPlayerInventoryDef::operator=(const CPlayerInventoryDef&) 0x00462E2D, ??4CPlayerInventoryDef@@QAEAAV0@ABV0@@Z (PDB name set + the landed operator= row's own definition at the jmp target).
// NF337 (2026-11-24): moved from the flattened __fastcall stand-in and its made-up `_Copy_impl` callee onto the PDB member; operator= is
//   declared out of line here, so the body is the one-instruction tail call retail has.
#include "rebuild_abi.h"

class CDefClassBase;
class CPlayerInventoryDef {                                // declarations only (no engine/CPlayerInventoryDef.h)
public:
    virtual void Copy(const CDefClassBase* other);              // vftable slot 19, 0x00462E28
    CPlayerInventoryDef& operator=(const CPlayerInventoryDef& other);   // out of line: 0x00462E2D
};

void CPlayerInventoryDef::Copy(const CDefClassBase* other)
{
    *this = *(const CPlayerInventoryDef*)other;
}
