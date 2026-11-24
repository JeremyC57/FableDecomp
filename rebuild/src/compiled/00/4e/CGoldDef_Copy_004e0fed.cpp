// Leaf: ?Copy@CGoldDef@@UAEXPBVCDefClassBase@@@Z
// CGoldDef::Copy(const CDefClassBase* other) -- copy another CGoldDef: the CDefClassBase part through its out-of-line operator=, then Gold
// Retail: Fable.exe (TLC, VC7.1) @ 0x004E0FED, thiscall, `ret 4`: `call 0x00431F10` (CDefClassBase::operator=, this = ecx, other pushed), then the
//   dword Gold +0x28 from other to this (the derived assignment is inline in retail: no out-of-line CGoldDef::operator= is called).
// Identity: the RTTI vftable placement of 0x004E0FED (ghidra_out/labels_rtti_port.tsv, slot 19 Copy) + Ego_r.pdb / FableWin.pdb name sets.
// Callee: CDefClassBase::operator=(const CDefClassBase&) 0x00431F10, ??4CDefClassBase@@QAEAAV0@ABV0@@Z (Ego_r.pdb + FableWin.pdb; engine/CDefClassBase.h).
// NF337 (2026-11-24): moved from the flattened __fastcall stand-in onto the PDB member on the shared class model (engine/CGoldDef.h FABLE_RETAIL_CGOLDDEF + the add-only FABLE_RETAIL_CGOLDDEF_COPY_DECLS Copy declaration); the explicit
//   base call + member copy is the inline assignment's expansion: the model's +0x25 pad array would be copied by an implicit operator= (retail copies +0x28 only), and the implicit form adds COMDAT operator= bodies (measured on three siblings, evidence/b4/implicit_form.txt).
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CGOLDDEF
#define FABLE_RETAIL_CGOLDDEF_COPY_DECLS
#include "rebuild_abi.h"
#include "engine/CGoldDef.h"

void CGoldDef::Copy(const CDefClassBase* other)
{
    CDefClassBase::operator=(*other);
    Gold = ((const CGoldDef*)other)->Gold;
}
