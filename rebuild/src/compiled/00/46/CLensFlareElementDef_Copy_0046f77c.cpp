// Leaf: ?Copy@CLensFlareElementDef@@UAEXPBVCDefClassBase@@@Z
// CLensFlareElementDef::Copy(const CDefClassBase* other) -- copy every member of another CLensFlareElementDef: `*this = *(const CLensFlareElementDef*)other;`
// Retail: Fable.exe (TLC, VC7.1) @ 0x0046F77C, 5 bytes: a thiscall tail call, `jmp` to CLensFlareElementDef::operator= 0x0046F6AF (emitted 0xCD bytes before it).
// Identity: the RTTI vftable placement of 0x0046F77C (ghidra_out/labels_rtti_port.tsv, slot 19 Copy) + Ego_r.pdb / FableWin.pdb name sets.
// Callee: CLensFlareElementDef::operator=(const CLensFlareElementDef&) 0x0046F6AF, ??4CLensFlareElementDef@@QAEAAV0@ABV0@@Z (Ego_r.pdb + FableWin.pdb; the landed row 0x00476B17 already binds it at 0x0046F6AF; the operator= row respells onto it in this unit).
// NF337 (2026-11-24): moved from the flattened __fastcall stand-in and its made-up `_Copy_impl` callee onto the PDB member; operator= is
//   declared out of line here, so the body is the one-instruction tail call retail has.
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CLENSFLAREELEMENTDEF_COPY_DECLS
#include "rebuild_abi.h"
#include "engine/CLensFlareElementDef.h"

void CLensFlareElementDef::Copy(const CDefClassBase* other)
{
    *this = *(const CLensFlareElementDef*)other;
}
