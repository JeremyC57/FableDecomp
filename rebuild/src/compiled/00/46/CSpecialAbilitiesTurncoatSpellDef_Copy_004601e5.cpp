// Leaf: ?Copy@CSpecialAbilitiesTurncoatSpellDef@@UAEXPBVCDefClassBase@@@Z
// CSpecialAbilitiesTurncoatSpellDef::Copy(const CDefClassBase* other) -- copy every member of another CSpecialAbilitiesTurncoatSpellDef: `*this = *(const CSpecialAbilitiesTurncoatSpellDef*)other;`
// Retail: Fable.exe (TLC, VC7.1) @ 0x004601E5, 5 bytes: a thiscall tail call, `jmp` to CSpecialAbilitiesTurncoatSpellDef::operator= 0x004601EA (emitted right after it).
// Identity: the RTTI vftable placement of 0x004601E5 (ghidra_out/labels_rtti_port.tsv, slot 19 Copy) + Ego_r.pdb / FableWin.pdb name sets.
// Callee: CSpecialAbilitiesTurncoatSpellDef::operator=(const CSpecialAbilitiesTurncoatSpellDef&) 0x004601EA, ??4CSpecialAbilitiesTurncoatSpellDef@@QAEAAV0@ABV0@@Z (PDB name set + the landed operator= row's own definition at the jmp target).
// NF337 (2026-11-24): moved from the flattened __fastcall stand-in and its made-up `_Copy_impl` callee onto the PDB member; operator= is
//   declared out of line here, so the body is the one-instruction tail call retail has.
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CSPECIALABILITIESTURNCOATSPELLDEF_COPY_DECLS
#include "rebuild_abi.h"
#include "engine/CSpecialAbilitiesTurncoatSpellDef.h"

void CSpecialAbilitiesTurncoatSpellDef::Copy(const CDefClassBase* other)
{
    *this = *(const CSpecialAbilitiesTurncoatSpellDef*)other;
}
