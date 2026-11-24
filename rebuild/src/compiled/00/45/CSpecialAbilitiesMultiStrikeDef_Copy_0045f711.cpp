// Leaf: ?Copy@CSpecialAbilitiesMultiStrikeDef@@UAEXPBVCDefClassBase@@@Z
// CSpecialAbilitiesMultiStrikeDef::Copy(const CDefClassBase* other) -- copy every member of another CSpecialAbilitiesMultiStrikeDef: `*this = *(const CSpecialAbilitiesMultiStrikeDef*)other;`
// Retail: Fable.exe (TLC, VC7.1) @ 0x0045F711, 5 bytes: a thiscall tail call, `jmp` to CSpecialAbilitiesMultiStrikeDef::operator= 0x0045F716 (emitted right after it).
// Identity: the RTTI vftable placement of 0x0045F711 (ghidra_out/labels_rtti_port.tsv, slot 19 Copy) + Ego_r.pdb / FableWin.pdb name sets.
// Callee: CSpecialAbilitiesMultiStrikeDef::operator=(const CSpecialAbilitiesMultiStrikeDef&) 0x0045F716, ??4CSpecialAbilitiesMultiStrikeDef@@QAEAAV0@ABV0@@Z (PDB name set + the landed operator= row's own definition at the jmp target).
// NF337 (2026-11-24): moved from the flattened __fastcall stand-in and its made-up `_Copy_impl` callee onto the PDB member; operator= is
//   declared out of line here, so the body is the one-instruction tail call retail has.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CSPECIALABILITIESMULTISTRIKEDEF_COPY_DECLS
#include "rebuild_abi.h"
#include "engine/CSpecialAbilitiesMultiStrikeDef.h"

void CSpecialAbilitiesMultiStrikeDef::Copy(const CDefClassBase* other)
{
    *this = *(const CSpecialAbilitiesMultiStrikeDef*)other;
}
