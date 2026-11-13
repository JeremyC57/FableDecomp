// CSurfaceCompositor::StartComposition(bool alphaTarget) -- begin an off-screen composition: open the render-state capture block,
// remember whether the target carries alpha, point the compositor's render target (z-buffer detached) at the destination surface and
// make it the display's render target (beginning the frame if rendering had not started, remembered in WasRendering), reset the
// default render states, save the current render window and set it to the surface's extents, set stage 0 to wrap / point sampling,
// apply the base blend modes and reset the layer count.
// Leaf: ?StartComposition@CSurfaceCompositor@@QAEX_N@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00F39730, 796 bytes, thiscall `ret 4`.
// Identity: Ego_r.pdb (0x00BDE060) and FableWin.pdb ?StartComposition@CSurfaceCompositor@@QAEX_N@Z; Ego_r order of the
//   CSurfaceCompositor.cpp TU (EndComposition, ctor, SetBlendMode, SetBaseBlendModes, StartComposition, CompositeQuad) equals retail.
// Callees: GFGetSystemManager 0x009A4EC0 (twice), CRenderTarget::DetachZBuffer 0x00A10E20, CRenderTarget::AttachTarget 0x00A10E60,
//   CDisplayManager::SetRenderTarget 0x009BF160 (result dropped), CDisplayManager::BeginRender 0x009BEF20 (result dropped),
//   CRenderStateManager::SetDefaultRenderStates 0x00A059E0, CRenderManagerCore::GetCurrentAWindow 0x00A0AAA0,
//   CSurface::GetDimensions 0x009F2F90 (twice), CRenderManagerCore::SetAWindow(const C2DBoxF&) 0x00A0AA80,
//   CSurfaceCompositor::SetBaseBlendModes 0x00F38FF0. Inline: CRenderStateCaptureBlock::Capture (StartCaptureBlock on the static
//   StateManager 0x013CB508), CDisplayManager::IsRendering (+0x214), the CRenderStateManager setters SetColourWriteEnable (4-bool,
//   mask 0xF), SetTextureUVModeU/V (D3DTextureUVModeTable 0x01396DC8) and SetTextureFilterModeMin/Mag (D3DTextureFilterModeTable 0x01396E58).
// Layout: PDB CSurfaceCompositor (0x30; no shared header -- modeled as the landed siblings 0x00F38A10 / 0x00F38990 do) and
//   CRenderTarget (no shared header; only the two called methods declared).
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_CRENDERSTATEMANAGER_COLOURWRITE4
#define FABLE_RETAIL_CSYSTEMMANAGER
#define FABLE_RETAIL_CDISPLAYMANAGER
#define FABLE_RETAIL_CDISPLAYMANAGER_RENDERTARGET
#define FABLE_RETAIL_CRENDERMANAGERCORE
#define FABLE_RETAIL_CTEXTURE
#define FABLE_RETAIL_CPIXELFORMAT
#define FABLE_RETAIL_C2DBOXF
#define FABLE_RETAIL_CSURFACE
#define FABLE_RETAIL_CSURFACE_BITMAP
#define FABLE_RETAIL_CRENDERSTATECAPTUREBLOCK_CAPTURE_INLINE
#include "engine/C2DExtentsI.h"
#include "engine/CSystemManager.h"
#include "engine/CDisplayManager.h"
#include "engine/CRenderManager.h"
#include "engine/CBaseClass.h"
#include "engine/CRenderStateCaptureBlock.h"
#include "engine/CSurface.h"
#include "engine/CCountedPointer.h"

class CRenderTarget : public CBaseClass {
public:
    void DetachZBuffer();                          // 0x00A10E20
    void AttachTarget(const CSurface& surface);    // 0x00A10E60
};
enum ECompositeBlendType { };
class CSurfaceCompositor : public CBaseClass {             // vtable 0x01301CF0, 0x30 bytes
public:
    virtual ~CSurfaceCompositor();
    void StartComposition(bool alphaTarget);
protected:
    void SetBaseBlendModes();                              // 0x00F38FF0
    ECompositeBlendType      CurrentBlend;                 // +0x04
    CRenderStateCaptureBlock StateCaptureBlock;            // +0x08
    bool                     WasRendering;                 // +0x09
    CSurface*                DestSurface;                  // +0x0c
    CCountedPointer<CRenderTarget> RenderTarget;           // +0x10
    C2DBoxF                  OldWindow;                    // +0x18
    long                     LayerCount;                   // +0x28
    bool                     AlphaTarget;                  // +0x2c
};

void CSurfaceCompositor::StartComposition(bool alphaTarget)
{
    StateCaptureBlock.Capture();
    AlphaTarget = alphaTarget;
    CDisplayManager* display = GFGetSystemManager()->GetDisplayManager();
    CRenderManager& core = GFGetSystemManager()->GetDisplayManager()->GetRenderManager();
    core.SetColourWriteEnable(true, true, true, true);
    RenderTarget->DetachZBuffer();
    RenderTarget->AttachTarget(*DestSurface);
    display->SetRenderTarget(*RenderTarget);
    WasRendering = true;
    if (!display->IsRendering()) {
        display->BeginRender();
        WasRendering = false;
    }
    core.SetDefaultRenderStates();
    OldWindow = core.GetCurrentAWindow();
    core.SetAWindow(C2DBoxF(0.0f, 0.0f, (float)DestSurface->GetDimensions().X, (float)DestSurface->GetDimensions().Y));
    core.SetTextureUVModeU(0, TEXTURE_UV_WRAP);
    core.SetTextureUVModeV(0, TEXTURE_UV_WRAP);
    core.SetTextureFilterModeMin(0, TEXTURE_FILTER_POINT);
    core.SetTextureFilterModeMag(0, TEXTURE_FILTER_POINT);
    SetBaseBlendModes();
    LayerCount = 0;
}