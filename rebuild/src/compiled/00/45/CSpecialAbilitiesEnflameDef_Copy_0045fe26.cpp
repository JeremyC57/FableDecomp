// Leaf: ?Copy@CSpecialAbilitiesEnflameDef@@UAEXPBVCDefClassBase@@@Z
// CSpecialAbilitiesEnflameDef::Copy(const CDefClassBase* other) -- copy every member of another CSpecialAbilitiesEnflameDef: `*this = *(const CSpecialAbilitiesEnflameDef*)other;`
// Retail: Fable.exe (TLC, VC7.1) @ 0x0045FE26, 5 bytes: a thiscall tail call, `jmp` to CSpecialAbilitiesEnflameDef::operator= 0x0045FE2B (emitted right after it).
// Identity: the RTTI vftable placement of 0x0045FE26 (ghidra_out/labels_rtti_port.tsv, slot 19 Copy) + Ego_r.pdb / FableWin.pdb name sets.
// Callee: CSpecialAbilitiesEnflameDef::operator=(const CSpecialAbilitiesEnflameDef&) 0x0045FE2B, ??4CSpecialAbilitiesEnflameDef@@QAEAAV0@ABV0@@Z (PDB name set + the landed operator= row's own definition at the jmp target).
// NF386 (2026-11-24; its T3_PORT_QUEUE line: a flatten / alias skip): moved from the flattened __fastcall stand-in and its made-up callee onto the PDB member; operator= is
//   declared out of line here, so the body is the one-instruction tail call retail has.
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CSPECIALABILITIESENFLAMEDEF_COPY_DECLS
#include "rebuild_abi.h"
#include "engine/CSpecialAbilitiesEnflameDef.h"

void CSpecialAbilitiesEnflameDef::Copy(const CDefClassBase* other)
{
    *this = *(const CSpecialAbilitiesEnflameDef*)other;
}
