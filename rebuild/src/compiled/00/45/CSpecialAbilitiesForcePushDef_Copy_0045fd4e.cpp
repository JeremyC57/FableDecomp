// Leaf: ?Copy@CSpecialAbilitiesForcePushDef@@UAEXPBVCDefClassBase@@@Z
// CSpecialAbilitiesForcePushDef::Copy(const CDefClassBase* other) -- copy every member of another CSpecialAbilitiesForcePushDef: `*this = *(const CSpecialAbilitiesForcePushDef*)other;`
// Retail: Fable.exe (TLC, VC7.1) @ 0x0045FD4E, 5 bytes: a thiscall tail call, `jmp` to CSpecialAbilitiesForcePushDef::operator= 0x0045FD53 (emitted right after it).
// Identity: the RTTI vftable placement of 0x0045FD4E (ghidra_out/labels_rtti_port.tsv, slot 19 Copy) + Ego_r.pdb / FableWin.pdb name sets.
// Callee: CSpecialAbilitiesForcePushDef::operator=(const CSpecialAbilitiesForcePushDef&) 0x0045FD53, ??4CSpecialAbilitiesForcePushDef@@QAEAAV0@ABV0@@Z (PDB name set + the landed operator= row's own definition at the jmp target).
// NF337 (2026-11-24): moved from the flattened __fastcall stand-in and its made-up `_Copy_impl` callee onto the PDB member; operator= is
//   declared out of line here, so the body is the one-instruction tail call retail has.
#define FABLE_RETAIL_CSPECIALABILITIESFORCEPUSHDEF
#define FABLE_RETAIL_CSPECIALABILITIESFORCEPUSHDEF_COPY_DECLS
#include "rebuild_abi.h"
#include "engine/CSpecialAbilitiesForcePushDef.h"

void CSpecialAbilitiesForcePushDef::Copy(const CDefClassBase* other)
{
    *this = *(const CSpecialAbilitiesForcePushDef*)other;
}
