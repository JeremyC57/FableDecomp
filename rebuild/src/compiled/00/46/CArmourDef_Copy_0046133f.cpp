// Leaf: ?Copy@CArmourDef@@UAEXPBVCDefClassBase@@@Z
// CArmourDef::Copy(const CDefClassBase* other) -- copy every member of another CArmourDef: `*this = *(const CArmourDef*)other;`
// Retail: Fable.exe (TLC, VC7.1) @ 0x0046133F, 5 bytes: a thiscall tail call, `jmp` to CArmourDef::operator= 0x00461344 (emitted right after it).
// Identity: the RTTI vftable placement of 0x0046133F (ghidra_out/labels_rtti_port.tsv, slot 19 Copy) + Ego_r.pdb / FableWin.pdb name sets.
// Callee: CArmourDef::operator=(const CArmourDef&) 0x00461344, ??4CArmourDef@@QAEAAV0@ABV0@@Z (PDB name set + the landed operator= row's own definition at the jmp target).
// NF337 (2026-11-24): moved from the flattened __fastcall stand-in and its made-up `_Copy_impl` callee onto the PDB member; operator= is
//   declared out of line here, so the body is the one-instruction tail call retail has.
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CARMOURDEF_COPY_DECLS
#include "rebuild_abi.h"
#include "engine/CArmourDef.h"

void CArmourDef::Copy(const CDefClassBase* other)
{
    *this = *(const CArmourDef*)other;
}
