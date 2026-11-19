// NPlayerGui::GFToScreen(const C2DVector& pos) -- the PC GUI-to-screen mapping is the identity: copy the input vector into the result
// Leaf: ?GFToScreen@NPlayerGui@@YI?AVC2DVector@@ABV2@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00643BE2, __fastcall (hidden C2DVector result in ecx, pos in edx), returns the result pointer.
// Name: Ego_r.pdb 0x00686AC7 + FableWin.pdb 0x020D908C ?GFToScreen@NPlayerGui@@YI?AVC2DVector@@ABV2@@Z (engine/NPlayerGui_Graphics.h
//   GFTOSCREEN_CALLED); the landed caller 0x00657580 binds it at its three call sites. NF95 2026-11-19 (q11292 / q11331): was the
//   stand-in free `CanMove(V2*, V2*)` (catalog NUISystem::CComponent::CanMove, a propagated label).
#pragma optimize("s",on)
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_NPLAYERGUI_GFTOSCREEN_CALLED
#include "engine/NPlayerGui_Graphics.h"
C2DVector FABLE_FASTCALL NPlayerGui::GFToScreen(const C2DVector& pos)
{
    return pos;
}
