// Leaf: ?Copy@CEnemyDef@@UAEXPBVCDefClassBase@@@Z
// CEnemyDef::Copy(const CDefClassBase* other) -- copy another CEnemyDef: the CDefClassBase part through its out-of-line operator=, then Faction
// Retail: Fable.exe (TLC, VC7.1) @ 0x004E064E, thiscall, `ret 4`: `call 0x00431F10` (CDefClassBase::operator=, this = ecx, other pushed), then the
//   dword Faction +0x28 from other to this (the derived assignment is inline in retail: no out-of-line CEnemyDef::operator= is called).
// Identity: the RTTI vftable placement of 0x004E064E (ghidra_out/labels_rtti_port.tsv, slot 19 Copy) + Ego_r.pdb / FableWin.pdb name sets.
// Callee: CDefClassBase::operator=(const CDefClassBase&) 0x00431F10, ??4CDefClassBase@@QAEAAV0@ABV0@@Z (Ego_r.pdb + FableWin.pdb; engine/CDefClassBase.h).
// NF337 (2026-11-24): moved from the flattened __fastcall stand-in onto the PDB member on the shared class model (engine/CEnemyDef.h FABLE_RETAIL_CENEMYDEF + the add-only FABLE_RETAIL_CENEMYDEF_COPY_DECLS Copy declaration); the explicit
//   base call + member copy is the inline assignment's expansion: `*this = *(const C*)other;` with the implicit operator= matches the Copy bytes but adds pick-any COMDAT ??4CEnemyDef and ??4CSubDefClassBase bodies (measured, NF337 g6: evidence/b4/implicit_form.txt).
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CENEMYDEF
#define FABLE_RETAIL_CENEMYDEF_COPY_DECLS
#include "rebuild_abi.h"
#include "engine/CEnemyDef.h"

void CEnemyDef::Copy(const CDefClassBase* other)
{
    CDefClassBase::operator=(*other);
    Faction = ((const CEnemyDef*)other)->Faction;
}
