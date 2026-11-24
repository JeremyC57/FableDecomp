// Leaf: ?Copy@CSpecialAbilitiesUnholyPowerDef@@UAEXPBVCDefClassBase@@@Z
// CSpecialAbilitiesUnholyPowerDef::Copy(const CDefClassBase* other) -- copy every member of another CSpecialAbilitiesUnholyPowerDef: `*this = *(const CSpecialAbilitiesUnholyPowerDef*)other;`
// Retail: Fable.exe (TLC, VC7.1) @ 0x004608C1, 5 bytes: a thiscall tail call, `jmp` to CSpecialAbilitiesUnholyPowerDef::operator= 0x004608C6 (emitted right after it).
// Identity: the RTTI vftable placement of 0x004608C1 (ghidra_out/labels_rtti_port.tsv, slot 19 Copy) + Ego_r.pdb / FableWin.pdb name sets.
// Callee: CSpecialAbilitiesUnholyPowerDef::operator=(const CSpecialAbilitiesUnholyPowerDef&) 0x004608C6, ??4CSpecialAbilitiesUnholyPowerDef@@QAEAAV0@ABV0@@Z (PDB name set + the landed operator= row's own definition at the jmp target).
// NF386 (2026-11-24; its T3_PORT_QUEUE line: a flatten / alias skip): moved from the flattened __fastcall stand-in and its made-up callee onto the PDB member; operator= is
//   declared out of line here, so the body is the one-instruction tail call retail has.
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CSPECIALABILITIESUNHOLYPOWERDEF_COPY_DECLS
#include "rebuild_abi.h"
#include "engine/CSpecialAbilitiesUnholyPowerDef.h"

void CSpecialAbilitiesUnholyPowerDef::Copy(const CDefClassBase* other)
{
    *this = *(const CSpecialAbilitiesUnholyPowerDef*)other;
}
