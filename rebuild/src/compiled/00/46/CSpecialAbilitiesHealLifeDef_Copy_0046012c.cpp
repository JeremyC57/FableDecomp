// Leaf: ?Copy@CSpecialAbilitiesHealLifeDef@@UAEXPBVCDefClassBase@@@Z
// CSpecialAbilitiesHealLifeDef::Copy(const CDefClassBase* other) -- copy every member of another CSpecialAbilitiesHealLifeDef: `*this = *(const CSpecialAbilitiesHealLifeDef*)other;`
// Retail: Fable.exe (TLC, VC7.1) @ 0x0046012C, 5 bytes: a thiscall tail call, `jmp` to CSpecialAbilitiesHealLifeDef::operator= 0x00460131 (emitted right after it).
// Identity: the RTTI vftable placement of 0x0046012C (ghidra_out/labels_rtti_port.tsv, slot 19 Copy) + Ego_r.pdb / FableWin.pdb name sets.
// Callee: CSpecialAbilitiesHealLifeDef::operator=(const CSpecialAbilitiesHealLifeDef&) 0x00460131, ??4CSpecialAbilitiesHealLifeDef@@QAEAAV0@ABV0@@Z (PDB name set + the landed operator= row's own definition at the jmp target).
// NF337 (2026-11-24): moved from the flattened __fastcall stand-in and its made-up `_Copy_impl` callee onto the PDB member; operator= is
//   declared out of line here, so the body is the one-instruction tail call retail has.
#define FABLE_RETAIL_CTCHEALLIFE_START_ORIGINAL
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CSPECIALABILITIESHEALLIFEDEF_COPY_DECLS
#include "rebuild_abi.h"
#include "engine/CSpecialAbilitiesHealLifeDef.h"

void CSpecialAbilitiesHealLifeDef::Copy(const CDefClassBase* other)
{
    *this = *(const CSpecialAbilitiesHealLifeDef*)other;
}
