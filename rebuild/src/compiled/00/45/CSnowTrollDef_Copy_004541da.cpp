// Leaf: ?Copy@CSnowTrollDef@@UAEXPBVCDefClassBase@@@Z
// CSnowTrollDef::Copy(const CDefClassBase* other) -- copy every member of another CSnowTrollDef: `*this = *(const CSnowTrollDef*)other;`
// Retail: Fable.exe (TLC, VC7.1) @ 0x004541DA, 5 bytes: a thiscall tail call, `jmp` to CSnowTrollDef::operator= 0x004541DF (emitted right after it).
// Identity: the RTTI vftable placement of 0x004541DA (ghidra_out/labels_rtti_port.tsv, slot 19 Copy) + Ego_r.pdb / FableWin.pdb name sets.
// Callee: CSnowTrollDef::operator=(const CSnowTrollDef&) 0x004541DF, ??4CSnowTrollDef@@QAEAAV0@ABV0@@Z (PDB name set + the landed operator= row's own definition at the jmp target).
// NF386 (2026-11-24; its T3_PORT_QUEUE line: a flatten / alias skip): moved from the flattened __fastcall stand-in and its made-up callee onto the PDB member; operator= is
//   declared out of line here, so the body is the one-instruction tail call retail has.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CSNOWTROLLDEF
#define FABLE_RETAIL_CSNOWTROLLDEF_COPY_DECLS
#include "rebuild_abi.h"
#include "engine/CSnowTrollDef.h"

void CSnowTrollDef::Copy(const CDefClassBase* other)
{
    *this = *(const CSnowTrollDef*)other;
}
