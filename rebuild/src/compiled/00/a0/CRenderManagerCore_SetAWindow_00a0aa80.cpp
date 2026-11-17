// CRenderManagerCore::SetAWindow(const C2DBoxF& window) -- forward the window to the display manager's float viewport.
// Leaf: ?SetAWindow@CRenderManagerCore@@QAEXABVC2DBoxF@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A0AA80, length 11 bytes (`mov ecx,[ecx+0x3A3C] ; jmp 0x009BF490`).
// Identity (NF56, round 7, queue q10418): Ego_r.pdb 0x00B74030 / FableWin.pdb 0x030EB2E0 ?SetAWindow@CRenderManagerCore@@QAEXABVC2DBoxF@@@Z
//   (the C2DBoxF overload, directly before GetCurrentAWindow 0x00A0AAA0 as in Ego_r); landed callers CEngineShadowRenderer::PreLayerRender
//   0x00B4F510 and CSurfaceCompositor 0x00F38930 already bind this spelling. engine/CRenderManagerCore.h declares it at this address.
//   Was the stand-in CRenderManagerCoreWindowView::SetAWindow (fable_render_window.h, a local view of CRenderManagerCore).
// Callee: CDisplayManager::SetViewport(const C2DBoxF&) @ 0x009BF490 (Ego_r.pdb 0x00B24470 / FableWin.pdb 0x0301F510) on DisplayManager +0x3A3C.
#define FABLE_RETAIL_CRENDERMANAGERCORE
#define FABLE_RETAIL_CDISPLAYMANAGER_SETVIEWPORT_BOXF
#define FABLE_RETAIL_C2DBOXF
#include "fable_compiler.h"
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/C2DBoxF.h"
#include "engine/CRenderManagerCore.h"
#include "engine/Direct3D9Types.h"
#include "engine/CDisplayManager.h"

void CRenderManagerCore::SetAWindow(const C2DBoxF& window)
{
    DisplayManager->SetViewport(window);
}
