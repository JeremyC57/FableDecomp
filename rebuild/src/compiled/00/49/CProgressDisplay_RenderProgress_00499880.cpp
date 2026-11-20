// CProgressDisplay::RenderProgress(const CCharString& textTag, float progress, bool showText, bool force) -- render one progress-screen frame: inside a render-state capture block, (re)start rendering, set a full-screen viewport, clear to black when full screen, draw the progress unless debug display is on, then restore the viewport, render target and rendering state
// Leaf: ?RenderProgress@CProgressDisplay@@AAEXABVCCharString@@M_N1@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00499880, 489 bytes + padding (`ret 10h`). Callees: GFGetSystemManager 0x009A4EC0, the render
// manager's 0x00A0AA60 / 0x00A0B560 / 0x009DA9F0 / 0x00A0AA90, the display manager's 0x009BEF20 (BeginRender, result dropped) / 0x009BE420 / 0x009BF220 / 0x009BEF50 /
// 0x009BEEB0 / 0x009BF160, CSystemManager 0x009A6460, CRenderTarget(const CSurface&, const CSurface&) 0x00A10DC0, CSurface::Uninitialise
// 0x009F2E20, ~CBaseClass 0x0099A2E0, CProgressDisplay 0x00498490, CRenderStateManager::RestoreCaptureBlock 0x00A05840. Names of the
// callees without a PDB/landed name are descriptive.
// Layout: CDisplayManager per Ego_r.pdb (PRenderManager +8, RenderTargetSurface +0xc, RenderTargetDepthBuffer +0x1c, ViewportBox +0x1f4,
// Rendering +0x214); CRenderStateManager as the landed CEngine::RenderColorFilteredQuad 0x00B25C00; CProgressDisplay as the sibling
// StartProgress 0x00499AA0 (pack(4): vfptr + doubles).
// Shape: the capture block is a scoped object (its restore runs after the render-target local's inline destructor); C2DBoxI has a
// member-wise user copy constructor (the viewport is copied field by field straight from the display manager); the display
// manager's Rendering flag is homed in the dead `force` slot.
// Naming (NF160, 2026-11-20, q4293 / q6169): the local CCharString / CSurface / CRenderTarget / CRenderStateManager / capture block /
//   C2DBoxI / C2DBoxF / CRGBColour / CRenderManager / CDisplayManager / CTextSettings / CSystemManager / CProgressDisplay models replaced by
//   the shared headers (the recipe of the landed siblings StartProgress 0x00499AA0 / DrawRetailDisplay 0x00498490 / StopProgress), so the
//   render-list call binds Ego_r.pdb 0x00B2E390 / FableWin.pdb ?Render2DDrawList@CRenderManager2D@@QAEX_N@Z (was the stand-in
//   CRenderManager::Flush). The other callees take the header spellings: 0x00A0AA60 is Ego_r.pdb 0x00B74010 / FableWin.pdb 0x030EB270
//   ?IsRendering@CRenderManagerCore@@QAE_NXZ (body `mov al,[ecx+3A54h] ; ret`, the Rendering flag +0x3A54; was the stand-in
//   IsDeviceLost), 0x00A0AA90 Ego_r.pdb 0x00B74040 / FableWin.pdb 0x030EB310 ?SetAWindow@CRenderManagerCore@@QAEXABVC2DBoxI@@@Z (the C2DBoxI
//   twin of SetAWindow(C2DBoxF) 0x00A0AA80, same `mov ecx,[ecx+3A3Ch] ; jmp` body, in PDB order after it; was the stand-in SetViewport),
//   0x00A0B560 ?SetVWindow@CRenderManagerCore@@QAEXABVC2DBoxF@@@Z, the viewport / render target through the inline FableWin.pdb accessors
//   GetViewport / GetRenderTarget, the +0x84 / +0x38 flag through CSystemManager::GetFontManager and the Ego_r.pdb inline
//   CFontManager::Get/SetLoadFontGlyphsAsynchronously, the capture block through the shared CRenderStateScopedCaptureBlock (its static
//   StateManager 0x013CB508), and 0x00498490 ?DrawRetailDisplay@CProgressDisplay@@AAEXABVCCharString@@M_N@Z.
// Naming (NF172, 2026-11-20, q4303 / q6167 / q11976 / q11977): NF160's shared-header port (draft blocked at DIFFER(476v489), the two inline
//   ~CSurface vtable stores missing) completed with FABLE_RETAIL_CSURFACE_COPYFROMMEMORY, the engine/CSurface.h mode whose virtual
//   ~CSurface is the inline body (the landed CEngineSkyRenderer::GetEnvironmentMap 0x00B665E0 recipe): ~CRenderTarget's expansion now
//   stores vtable 0x0122F84C into each CSurface member before Uninitialise 0x009F2E20, as retail does at 0x00499A12. The own leaf moves
//   from the public stand-in QAE to the private PDB form (Ego_r.pdb 0x004A4470 / FableWin.pdb 0x018B1460, AAE); its other referencers
//   0x00499AA0 / 0x00499D90 already bind it and 0x00499C50 is respelled in place in this hand-back.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CTEXTURE
#define FABLE_RETAIL_CPIXELFORMAT
#define FABLE_RETAIL_C2DBOXI
#define FABLE_RETAIL_C2DBOXI_COPY
#define FABLE_RETAIL_CSURFACE
#define FABLE_RETAIL_CSURFACE_COPYFROMMEMORY
#define FABLE_RETAIL_CSURFACE_FORCEINLINE_DTOR
#define FABLE_RETAIL_C2DBOXF
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_CSYSTEMMANAGER
#define FABLE_RETAIL_CSYSTEMMANAGER_FONTMANAGER
#define FABLE_RETAIL_CFONTMANAGER_ASYNC
#define FABLE_RETAIL_CFONTMANAGER_GETASYNC
#define FABLE_RETAIL_CDISPLAYMANAGER
#define FABLE_RETAIL_CDISPLAYMANAGER_RENDERTARGET
#define FABLE_RETAIL_CDISPLAYMANAGER_GETVIEWPORT
#define FABLE_RETAIL_CDISPLAYMANAGER_GETRENDERTARGET
#define FABLE_RETAIL_CDISPLAYMANAGER_BACKSCREEN
#define FABLE_RETAIL_CRENDERMANAGERCORE
#include "fable_string.h"
#include "engine/C2DBoxI.h"
#include "engine/C2DBoxF.h"
#include "engine/CRGBColour.h"
#include "engine/CTexture.h"
#include "engine/CSystemManager.h"
#include "engine/CFontManager.h"
#include "engine/CRenderManager.h"
#include "engine/CDisplayManager.h"
#include "engine/CRenderStateCaptureBlock.h"
#include "engine/CProgressDisplay.h"

CSystemManager* __fastcall GFGetSystemManager();           // 0x009A4EC0
extern double g_ProgressScreenWidth;                        // 0x013961E8 (descriptive)
extern double g_ProgressScreenHeight;                       // 0x013961F0 (descriptive)

void CProgressDisplay::RenderProgress(const CCharString& textTag, float progress, bool showText, bool force)
{
    CDisplayManager* display = GFGetSystemManager()->GetDisplayManager();
    if (!display)
        return;
    CRenderManager* renderer = &display->GetRenderManager();
    if (!renderer)
        return;
    if (!force && renderer->IsRendering())
        return;
    bool wasRendering = display->IsRendering();
    if (DebugDisplay && !wasRendering)
        GFGetSystemManager()->Update();
    CRenderStateScopedCaptureBlock captureBlock;
    bool savedFlag = false;
    if (FullScreen) {
        CFontManager* fonts = GFGetSystemManager()->GetFontManager();
        savedFlag = fonts->GetLoadFontGlyphsAsynchronously();
        fonts->SetLoadFontGlyphsAsynchronously(false);
    }
    if (!wasRendering)
        display->BeginRender();
    C2DBoxI oldViewport = display->GetViewport();
    CRenderTarget oldTarget = display->GetRenderTarget();
    renderer->SetVWindow(C2DBoxF(0.0f, 0.0f, (float)g_ProgressScreenWidth, (float)g_ProgressScreenHeight));
    if (FullScreen)
        display->ClearRenderTarget(CRGBColour(0, 0, 0, 0xff), 7);
    display->SetRenderTargetAsBackScreen();
    if (!DebugDisplay)
        DrawRetailDisplay(textTag, progress, showText);
    renderer->Render2DDrawList(true);
    display->EndRender();
    display->SwapScreens();
    renderer->SetAWindow(oldViewport);
    display->SetRenderTarget(oldTarget);
    if (wasRendering)
        display->BeginRender();
    if (FullScreen)
        GFGetSystemManager()->GetFontManager()->SetLoadFontGlyphsAsynchronously(savedFlag);
}
