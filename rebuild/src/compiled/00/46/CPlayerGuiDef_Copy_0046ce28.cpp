// CPlayerGuiDef::Copy(const CDefClassBase*) -- the slot-19 def copy: a 5-byte `jmp` to the implicit operator= COMDAT 0x0046CE2D
// Leaf: ?Copy@CPlayerGuiDef@@UAEXPBVCDefClassBase@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0046CE28 (5 bytes: `jmp 0x0046CE2D`).
// NF39 (2026-11-16): moved off its free __fastcall stand-in (CPlayerGuiDef_Copy -> CPlayerGuiDef_Copy_impl) onto the shared
// engine/CPlayerGuiDef.h (FABLE_RETAIL_CPLAYERGUIDEF_CLASS; the header names slot 19 Copy 0x0046CE28): FableWin.exe.signatures.tsv
// ?Copy@CPlayerGuiDef@@UAEXPBVCDefClassBase@@@Z + vtable 0x012352DC slot 19. The same source as the landed 0x0046CE2D row (the operator=
// COMDAT this thunk tail-calls); this row grades the Copy symbol of that object.
#include "fable_compiler.h"
#pragma optimize("s", on)
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_STLP_VECTOR_ASSIGN
#define FABLE_RETAIL_CPLAYERGUIDEF_CLASS
#include "engine/CPlayerGuiDef.h"

void CPlayerGuiDef::Copy(const CDefClassBase* def)
{
    *this = *static_cast<const CPlayerGuiDef*>(def);
}
