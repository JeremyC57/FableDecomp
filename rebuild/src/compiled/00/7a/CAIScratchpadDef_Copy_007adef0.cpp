// Leaf: ?Copy@CAIScratchpadDef@@UAEXPBVCDefClassBase@@@Z
// CAIScratchpadDef::Copy(const CDefClassBase* other) -- copy every member of another CAIScratchpadDef: `*this = *(const CAIScratchpadDef*)other;`
// Retail: Fable.exe (TLC, VC7.1) @ 0x007ADEF0, 5 bytes: a thiscall tail call, `jmp` to CAIScratchpadDef::operator= 0x007ADF00 (emitted right after it).
// Identity: the RTTI vftable placement of 0x007ADEF0 (ghidra_out/labels_rtti_port.tsv, slot 19 Copy) + Ego_r.pdb / FableWin.pdb name sets.
// Callee: CAIScratchpadDef::operator=(const CAIScratchpadDef&) 0x007ADF00, ??4CAIScratchpadDef@@QAEAAV0@ABV0@@Z (PDB name set + the landed operator= row's own definition at the jmp target).
// NF386 (2026-11-24; its T3_PORT_QUEUE line: a flatten / alias skip): moved from the flattened __fastcall stand-in and its made-up callee onto the PDB member; operator= is
//   declared out of line here, so the body is the one-instruction tail call retail has.
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CAISCRATCHPADDEF_VIRTUAL_MODEL
#define FABLE_RETAIL_CAISCRATCHPADDEF_COPY_DECLS
#include "rebuild_abi.h"
#include "engine/CAIScratchpadDef.h"

void CAIScratchpadDef::Copy(const CDefClassBase* other)
{
    *this = *(const CAIScratchpadDef*)other;
}
