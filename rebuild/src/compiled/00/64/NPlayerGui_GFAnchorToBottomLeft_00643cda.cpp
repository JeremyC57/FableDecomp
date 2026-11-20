// NPlayerGui::GFAnchorToBottomLeft(const C2DVector& pos) -- a 640x480 GUI position anchored to the bottom-left corner of the real render target
// Leaf: ?GFAnchorToBottomLeft@NPlayerGui@@YI?AVC2DVector@@ABV2@@Z
// Naming wave 18: PDisplayManager is GPDisplayManager (?GPDisplayManager@@3PAVCDisplayManager@@A); ChatGPT pass 10 audit, raw Ego_r public.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00643CDA, 50 bytes, plain `ret` (__fastcall: hidden C2DVector result in ecx,
// pos in edx). Size-optimised NPlayerGui TU; this one has no frame pointer (VC's size heuristic picks the ebp frame only for
// its siblings with more frame traffic).
// Identity: Ego_r.pdb `?GFAnchorToBottomLeft@NPlayerGui@@YI?AVC2DVector@@ABV2@@Z`.
// Callees: CDisplayManager::GetRenderTargetDimensions() 0x009BEDC0 on the display manager 0x013B8390. Float 480.0f from
// pool 0x01238874.
// Shape: C2DVector(pos.X, height + pos.Y - 480); the Y expression (the right-hand argument) is evaluated first.
// Re-port (audit h3c2b, 2026-11-20): the local C2DVector / C2DBoxF / C2DExtentsI / CDisplayManager models are replaced by the shared
// engine/C2DVector.h, engine/CDisplayManager.h (C2DExtentsI is X / Y in every PDB: the earlier `.Height` was a local spelling) and the
// declaration of engine/NPlayerGui_Graphics.h (FABLE_RETAIL_NPLAYERGUI_ANCHORS). The definition below is the text the same-TU strong
// copy in 0x00646762 NPlayerGui::CStatBar::DrawStatBarOnLeft carries verbatim (GOTCHAS line 455).
#pragma optimize("s", on)
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_NPLAYERGUI_ANCHORS
#define FABLE_RETAIL_CDISPLAYMANAGER
#define FABLE_RETAIL_SCOPED_PTR
#include "engine/C2DVector.h"
#include "engine/NPlayerGui_Graphics.h"
#include "engine/Win32Types.h"
#include "engine/Direct3D9Types.h"
#include "engine/CDisplayManager.h"
extern CDisplayManager* GPDisplayManager;                                       // 0x013B8390 ?GPDisplayManager@@3PAVCDisplayManager@@A

C2DVector FABLE_FASTCALL NPlayerGui::GFAnchorToBottomLeft(const C2DVector& pos)
{
    return C2DVector(pos.X, (float)GPDisplayManager->GetRenderTargetDimensions().Y + pos.Y - 480.0f);
}