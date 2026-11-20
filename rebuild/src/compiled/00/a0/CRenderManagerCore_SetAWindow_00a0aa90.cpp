// CRenderManagerCore::SetAWindow(const C2DBoxI& window) -- forward the window to the display manager's integer viewport.
// Leaf: ?SetAWindow@CRenderManagerCore@@QAEXABVC2DBoxI@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A0AA90, 11 bytes (`mov ecx,[ecx+0x3A3C] ; jmp 0x009BEF80`).
// Naming (NF172, 2026-11-20, q11977): Ego_r.pdb 0x00B74040 + FableWin.pdb 0x030EB310 ?SetAWindow@CRenderManagerCore@@QAEXABVC2DBoxI@@@Z,
//   the C2DBoxI twin of SetAWindow(const C2DBoxF&) 0x00A0AA80 (Ego_r 0x00B74030, the same `mov ecx,[ecx+3A3Ch] ; jmp` body) and in PDB
//   order after it; declared in engine/CRenderManagerCore.h. Was the stand-in CTCInventoryBaseViewportView::SetViewport_00A0AA90 (catalog
//   _global::Forward_SetViewport_via0x3a3c_00a0aa90). The sole landed caller, 0x00499880 CProgressDisplay::RenderProgress, binds this
//   spelling in the same hand-back.
// Callee: CDisplayManager::SetViewport(const C2DBoxI&) @ 0x009BEF80 (Ego_r.pdb 0x00B23C90 / FableWin.pdb 0x0301F590) on DisplayManager
//   +0x3A3C, the spelling the row bound before.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CTEXTURE
#define FABLE_RETAIL_CPIXELFORMAT
#define FABLE_RETAIL_C2DBOXI
#define FABLE_RETAIL_CSURFACE
#define FABLE_RETAIL_C2DBOXF
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_CDISPLAYMANAGER
#define FABLE_RETAIL_CDISPLAYMANAGER_SETVIEWPORT
#define FABLE_RETAIL_CRENDERMANAGERCORE
#include "fable_string.h"
#include "engine/C2DBoxI.h"
#include "engine/C2DBoxF.h"
#include "engine/CTexture.h"
#include "engine/CRenderManager.h"
#include "engine/CDisplayManager.h"

void CRenderManagerCore::SetAWindow(const C2DBoxI& window)
{
    DisplayManager->SetViewport(window);
}
