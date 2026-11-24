// Leaf: ?Copy@CExpressionSubDef@@UAEXPBVCDefClassBase@@@Z
// CExpressionSubDef::Copy(const CDefClassBase* other) -- copy another CExpressionSubDef: the CDefClassBase part through its out-of-line operator=, then ExpressionDef
// Retail: Fable.exe (TLC, VC7.1) @ 0x004E0CA7, thiscall, `ret 4`: `call 0x00431F10` (CDefClassBase::operator=, this = ecx, other pushed), then the
//   dword ExpressionDef +0x28 from other to this (the derived assignment is inline in retail: no out-of-line CExpressionSubDef::operator= is called).
// Identity: the RTTI vftable placement of 0x004E0CA7 (ghidra_out/labels_rtti_port.tsv, slot 19 Copy) + Ego_r.pdb / FableWin.pdb name sets.
// Callee: CDefClassBase::operator=(const CDefClassBase&) 0x00431F10, ??4CDefClassBase@@QAEAAV0@ABV0@@Z (Ego_r.pdb + FableWin.pdb; engine/CDefClassBase.h).
// NF337 (2026-11-24): moved from the flattened __fastcall stand-in onto the PDB member on the shared class model (engine/CExpressionSubDef.h FABLE_RETAIL_CEXPRESSIONSUBDEF + the add-only FABLE_RETAIL_CEXPRESSIONSUBDEF_COPY_DECLS Copy declaration); the explicit
//   base call + member copy is the inline assignment's expansion: `*this = *(const C*)other;` with the implicit operator= matches the Copy bytes but adds pick-any COMDAT ??4CExpressionSubDef and ??4CSubDefClassBase bodies (measured, NF337 g6: evidence/b4/implicit_form.txt).
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CEXPRESSIONSUBDEF
#define FABLE_RETAIL_CEXPRESSIONSUBDEF_COPY_DECLS
#include "rebuild_abi.h"
#include "engine/CExpressionSubDef.h"

void CExpressionSubDef::Copy(const CDefClassBase* other)
{
    CDefClassBase::operator=(*other);
    ExpressionDef = ((const CExpressionSubDef*)other)->ExpressionDef;
}
