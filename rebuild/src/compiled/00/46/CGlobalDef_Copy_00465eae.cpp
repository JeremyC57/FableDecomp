// Leaf: ?Copy@CGlobalDef@@UAEXPBVCDefClassBase@@@Z
// CGlobalDef::Copy(const CDefClassBase* other) -- copy every member of another CGlobalDef: `*this = *(const CGlobalDef*)other;`
// Retail: Fable.exe (TLC, VC7.1) @ 0x00465EAE, 5 bytes: a thiscall tail call, `jmp` to CGlobalDef::operator= 0x00465EB3 (emitted right after it).
// Identity: the RTTI vftable placement of 0x00465EAE (ghidra_out/labels_rtti_port.tsv, slot 19 Copy) + Ego_r.pdb / FableWin.pdb name sets.
// Callee: CGlobalDef::operator=(const CGlobalDef&) 0x00465EB3, ??4CGlobalDef@@QAEAAV0@ABV0@@Z (PDB name set + the landed operator= row's own definition at the jmp target).
// NF337 (2026-11-24): moved from the flattened __fastcall stand-in and its made-up `_Copy_impl` callee onto the PDB member; operator= is
//   declared out of line here, so the body is the one-instruction tail call retail has.
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "rebuild_abi.h"
#include "engine/CGlobalDef.h"

void CGlobalDef::Copy(const CDefClassBase* other)
{
    *this = *(const CGlobalDef*)other;
}
