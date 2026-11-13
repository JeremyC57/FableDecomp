// CPlayerGuiDef::operator=(const CPlayerGuiDef& other) -- compiler-generated copy assignment: CParentDefClassBase::operator=, then
//   every CPlayerGuiDef member +0x3C..+0xAB0 in declaration order (scalars / C2DVector / CRGBColour as dwords, the C3DVector
//   DiffuseVector as a 12-byte block, containers and strings through their own operator=), returns *this.
// Leaf: ??4CPlayerGuiDef@@QAEAAV0@ABV0@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0046CE2D, 7312 bytes (size-optimised def TU), thiscall, `ret 4`, returns this.
// Identity (proven): Ego_r.pdb ??4CPlayerGuiDef@@QAEAAV0@ABV0@@Z (0x00480662, 7318 bytes); its only caller is CPlayerGuiDef::Copy
//   0x0046CE28 (a `jmp` to it), slot 19 of vtable 0x012352DC (?Copy@CPlayerGuiDef@@UAEXPBVCDefClassBase@@@Z, FableWin.pdb). The
//   manifest label CParentDefClassBase::operator= is propagated from the base call.
// Callees: CParentDefClassBase::operator= 0x00431ED0; std::vector<long>::operator= 0x00454886; std::vector<C2DVector>::operator=
//   0x0046EABD; std::vector<float>::operator= 0x0045EF2D; CCharString::operator= 0x0099EFB0; std::vector<CRGBColour>::operator=
//   0x0045BAAA; std::vector<CQuickAccessItemButtonGuiDef>::operator= 0x0046ED15; std::vector<CAbilityButtonGuiDef>::operator=
//   0x0046F06A; std::vector<CCharString>::operator= 0x00432EE9; _Rb_tree<CCharString,pair<const CCharString,long>,..>::operator=
//   0x00432B38 (map<CCharString,long> ScriptSprites / MiniMapGraphics); _Rb_tree<..,EGameAction,..>::operator= 0x0046EBE4.
// Shape: the Copy definition below only makes the compiler emit the implicit operator= (Copy itself is the landed row 0x0046CE28).
// Headers: engine/CPlayerGuiDef.h opt-in FABLE_RETAIL_CPLAYERGUIDEF_CLASS (whole retail class), FABLE_RETAIL_DEF_CLASS_BASE,
//   FABLE_RETAIL_STLP_VECTOR_ASSIGN (vector operator= declared out of line, as retail calls it).
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
