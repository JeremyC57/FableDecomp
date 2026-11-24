// Leaf: ?Copy@CSpecialAbilitiesBulletTimeDef@@UAEXPBVCDefClassBase@@@Z
// CSpecialAbilitiesBulletTimeDef::Copy(const CDefClassBase* other) -- copy every member of another CSpecialAbilitiesBulletTimeDef: `*this = *(const CSpecialAbilitiesBulletTimeDef*)other;`
// Retail: Fable.exe (TLC, VC7.1) @ 0x0045FABC, 5 bytes: a thiscall tail call, `jmp` to CSpecialAbilitiesBulletTimeDef::operator= 0x0045FAC1 (emitted right after it).
// Identity: the RTTI vftable placement of 0x0045FABC (ghidra_out/labels_rtti_port.tsv, slot 19 Copy) + Ego_r.pdb / FableWin.pdb name sets.
// Callee: CSpecialAbilitiesBulletTimeDef::operator=(const CSpecialAbilitiesBulletTimeDef&) 0x0045FAC1, ??4CSpecialAbilitiesBulletTimeDef@@QAEAAV0@ABV0@@Z (PDB name set + the landed operator= row's own definition at the jmp target).
// NF337 (2026-11-24): moved from the flattened __fastcall stand-in and its made-up `_Copy_impl` callee onto the PDB member; operator= is
//   declared out of line here, so the body is the one-instruction tail call retail has.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CSPECIALABILITIESBULLETTIMEDEF_COPY_DECLS
#include "rebuild_abi.h"
#include "engine/CSpecialAbilitiesBulletTimeDef.h"

void CSpecialAbilitiesBulletTimeDef::Copy(const CDefClassBase* other)
{
    *this = *(const CSpecialAbilitiesBulletTimeDef*)other;
}
