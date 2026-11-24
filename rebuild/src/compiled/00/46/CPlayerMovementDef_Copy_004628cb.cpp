// Leaf: ?Copy@CPlayerMovementDef@@UAEXPBVCDefClassBase@@@Z
// CPlayerMovementDef::Copy(const CDefClassBase* other) -- copy every member of another CPlayerMovementDef: `*this = *(const CPlayerMovementDef*)other;`
// Retail: Fable.exe (TLC, VC7.1) @ 0x004628CB, 5 bytes: a thiscall tail call, `jmp` to CPlayerMovementDef::operator= 0x004628D0 (emitted right after it).
// Identity: the RTTI vftable placement of 0x004628CB (ghidra_out/labels_rtti_port.tsv, slot 19 Copy) + Ego_r.pdb / FableWin.pdb name sets.
// Callee: CPlayerMovementDef::operator=(const CPlayerMovementDef&) 0x004628D0, ??4CPlayerMovementDef@@QAEAAV0@ABV0@@Z (PDB name set + the landed operator= row's own definition at the jmp target).
// NF337 (2026-11-24): moved from the flattened __fastcall stand-in and its made-up `_Copy_impl` callee onto the PDB member; operator= is
//   declared out of line here, so the body is the one-instruction tail call retail has.
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CPLAYERMOVEMENTDEF_COPY_DECLS
#include "rebuild_abi.h"
#include "engine/CPlayerMovementDef.h"

void CPlayerMovementDef::Copy(const CDefClassBase* other)
{
    *this = *(const CPlayerMovementDef*)other;
}
