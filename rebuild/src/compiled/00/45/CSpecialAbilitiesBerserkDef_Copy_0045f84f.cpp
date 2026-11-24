// Leaf: ?Copy@CSpecialAbilitiesBerserkDef@@UAEXPBVCDefClassBase@@@Z
// CSpecialAbilitiesBerserkDef::Copy(const CDefClassBase* other) -- copy every member of another CSpecialAbilitiesBerserkDef: `*this = *(const CSpecialAbilitiesBerserkDef*)other;`
// Retail: Fable.exe (TLC, VC7.1) @ 0x0045F84F, 5 bytes: a thiscall tail call, `jmp` to CSpecialAbilitiesBerserkDef::operator= 0x0045F854 (emitted right after it).
// Identity: the RTTI vftable placement of 0x0045F84F (ghidra_out/labels_rtti_port.tsv, slot 19 Copy) + Ego_r.pdb / FableWin.pdb name sets.
// Callee: CSpecialAbilitiesBerserkDef::operator=(const CSpecialAbilitiesBerserkDef&) 0x0045F854, ??4CSpecialAbilitiesBerserkDef@@QAEAAV0@ABV0@@Z (PDB name set + the landed operator= row's own definition at the jmp target).
// NF337 (2026-11-24): moved from the flattened __fastcall stand-in and its made-up `_Copy_impl` callee onto the PDB member; operator= is
//   declared out of line here, so the body is the one-instruction tail call retail has.
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CSPECIALABILITIESBERSERKDEF_COPY_DECLS
#include "rebuild_abi.h"
#include "engine/CSpecialAbilitiesBerserkDef.h"

void CSpecialAbilitiesBerserkDef::Copy(const CDefClassBase* other)
{
    *this = *(const CSpecialAbilitiesBerserkDef*)other;
}
