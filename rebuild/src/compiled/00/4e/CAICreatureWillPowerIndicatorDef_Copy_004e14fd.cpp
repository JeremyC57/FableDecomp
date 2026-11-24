// Leaf: ?Copy@CAICreatureWillPowerIndicatorDef@@UAEXPBVCDefClassBase@@@Z
// CAICreatureWillPowerIndicatorDef::Copy(const CDefClassBase* other) -- copy another CAICreatureWillPowerIndicatorDef: the CDefClassBase part through its out-of-line operator=, then RechargeTime
// Retail: Fable.exe (TLC, VC7.1) @ 0x004E14FD, thiscall, `ret 4`: `call 0x00431F10` (CDefClassBase::operator=, this = ecx, other pushed), then the
//   dword RechargeTime +0x28 from other to this (the derived assignment is inline in retail: no out-of-line CAICreatureWillPowerIndicatorDef::operator= is called).
// Identity: the RTTI vftable placement of 0x004E14FD (ghidra_out/labels_rtti_port.tsv, slot 19 Copy) + Ego_r.pdb / FableWin.pdb name sets.
// Callee: CDefClassBase::operator=(const CDefClassBase&) 0x00431F10, ??4CDefClassBase@@QAEAAV0@ABV0@@Z (Ego_r.pdb + FableWin.pdb; engine/CDefClassBase.h).
// NF337 (2026-11-24): moved from the flattened __fastcall stand-in onto the PDB member on the shared class model (engine/CAICreatureWillPowerIndicatorDef.h FABLE_RETAIL_CAICREATUREWILLPOWERINDICATORDEF_VIRTUAL_MODEL, which declares Copy; no header edit); the explicit
//   base call + member copy is the inline assignment's expansion: the model's +0x25 pad array would be copied by an implicit operator= (retail copies +0x28 only), and the implicit form adds COMDAT operator= bodies (measured on three siblings, evidence/b4/implicit_form.txt).
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CAICREATUREWILLPOWERINDICATORDEF_VIRTUAL_MODEL
#include "rebuild_abi.h"
#include "engine/CAICreatureWillPowerIndicatorDef.h"

void CAICreatureWillPowerIndicatorDef::Copy(const CDefClassBase* other)
{
    CDefClassBase::operator=(*other);
    RechargeTime = ((const CAICreatureWillPowerIndicatorDef*)other)->RechargeTime;
}
