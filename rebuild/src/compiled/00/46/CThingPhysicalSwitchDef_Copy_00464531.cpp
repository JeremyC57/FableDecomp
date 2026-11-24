// Leaf: ?Copy@CThingPhysicalSwitchDef@@UAEXPBVCDefClassBase@@@Z
// CThingPhysicalSwitchDef::Copy(const CDefClassBase* other) -- copy every member of another CThingPhysicalSwitchDef: `*this = *(const CThingPhysicalSwitchDef*)other;`
// Retail: Fable.exe (TLC, VC7.1) @ 0x00464531, 5 bytes: a thiscall tail call, `jmp` to CThingPhysicalSwitchDef::operator= 0x00464536 (emitted right after it).
// Identity: the RTTI vftable placement of 0x00464531 (ghidra_out/labels_rtti_port.tsv, slot 19 Copy) + Ego_r.pdb / FableWin.pdb name sets.
// Callee: CThingPhysicalSwitchDef::operator=(const CThingPhysicalSwitchDef&) 0x00464536, ??4CThingPhysicalSwitchDef@@QAEAAV0@ABV0@@Z (Ego_r.pdb + FableWin.pdb; the operator= row respells onto it from ??4CThingBaseDef, the 0x00463950 symbol, in this unit).
// NF337 (2026-11-24): moved from the flattened __fastcall stand-in and its made-up `_Copy_impl` callee onto the PDB member; operator= is
//   declared out of line here, so the body is the one-instruction tail call retail has.
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTHINGPHYSICALSWITCHDEF_COPY_DECLS
#include "rebuild_abi.h"
#include "engine/CThingPhysicalSwitchDef.h"

void CThingPhysicalSwitchDef::Copy(const CDefClassBase* other)
{
    *this = *(const CThingPhysicalSwitchDef*)other;
}
