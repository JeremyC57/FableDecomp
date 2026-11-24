// Leaf: ?Copy@CThingObjectDef@@UAEXPBVCDefClassBase@@@Z
// CThingObjectDef::Copy(const CDefClassBase* other) -- copy every member of another CThingObjectDef: `*this = *(const CThingObjectDef*)other;`
// Retail: Fable.exe (TLC, VC7.1) @ 0x004642F5, 5 bytes: a thiscall tail call, `jmp` to CThingObjectDef::operator= 0x004642FA (emitted right after it).
// Identity: the RTTI vftable placement of 0x004642F5 (ghidra_out/labels_rtti_port.tsv, slot 19 Copy) + Ego_r.pdb / FableWin.pdb name sets.
// Callee: CThingObjectDef::operator=(const CThingObjectDef&) 0x004642FA, ??4CThingObjectDef@@QAEAAV0@ABV0@@Z (PDB name set + the landed operator= row's own definition at the jmp target).
// NF386 (2026-11-24; its T3_PORT_QUEUE line: a flatten / alias skip): moved from the flattened __fastcall stand-in and its made-up callee onto the PDB member; operator= is
//   declared out of line here, so the body is the one-instruction tail call retail has.
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTHINGBASEDEF
#define FABLE_RETAIL_CTHINGOBJECTDEF_VALIDATE
#include "rebuild_abi.h"
#include "engine/CThingObjectDef.h"

void CThingObjectDef::Copy(const CDefClassBase* other)
{
    *this = *(const CThingObjectDef*)other;
}
