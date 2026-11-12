// NPlayerGui::CDrawBase::ScaleAlpha(unsigned char alpha) const -- the drawer's alpha scaled by the player GUI's fade: (unsigned char)(alpha * PlayerGui->Alpha)
// Leaf: ?ScaleAlpha@CDrawBase@NPlayerGui@@QBEEE@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00643D5D, 31 bytes, thiscall `ret 4`, ebp frame.
// Flags: none
// Name: Ego_r.pdb 0x00686C60 + FableWin.pdb 0x020D93CE ?ScaleAlpha@CDrawBase@NPlayerGui@@QBEEE@Z; the landed sprite drawers call it on
//   their CDrawBase `this` with one byte argument (DrawButton 0x00645099, DrawIcon 0x00645292, CDrawMeleeActionButtonEffects::Draw
//   0x00646580; reloc_bindings first caller 0x00645099). Name-fix pass (2026-11-12): was the free __fastcall stand-in Cons_val_CEntry
//   (a propagated label) on local Outer / Inner structs; now the member on engine/NPlayerGui_CDrawBase.h (declared by the HANDLES opt-in; this row uses the declaration-only SCALEALPHA opt-in, hdr/README.md)
//   and engine/CPlayerGui.h (Alpha +0x2A0, the same inline body as CPlayerGui::ScaleAlpha).
// Callees: __ftol2 0x00BFEA70 (the float-to-integer conversion).
// Shape: the byte is widened into the dead argument slot and loaded with `fild`; ebp frame as in the other NPlayerGui drawer TU rows.
#pragma optimize("y", off)
#pragma optimize("s", on)
#define FABLE_RETAIL_NPLAYERGUI_CDRAWBASE_SCALEALPHA
#include "engine/CPlayerGui.h"
#include "engine/NPlayerGui_CDrawBase.h"

unsigned char NPlayerGui::CDrawBase::ScaleAlpha(unsigned char alpha) const
{
    return (unsigned char)(alpha * PlayerGui->Alpha);
}
