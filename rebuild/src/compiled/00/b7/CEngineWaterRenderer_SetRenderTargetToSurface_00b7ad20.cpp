// CEngineWaterRenderer::SetRenderTargetToSurface(CRenderTarget& target, const CSurface& surface) -- attach the surface to the render target, make it the display manager's render target, and set a full-surface viewport in virtual coordinates
// Leaf: ?SetRenderTargetToSurface@CEngineWaterRenderer@@AAEXAAVCRenderTarget@@ABVCSurface@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B7AD20, 172 bytes. __fastcall (this unused), `ret 8`.
// Identity: FableWin.pdb `?SetRenderTargetToSurface@CEngineWaterRenderer@@AAEXAAVCRenderTarget@@ABVCSurface@@@Z`; the
//   body calls CRenderTarget::AttachTarget(const CSurface&) on arg 1 and CSurface::GetDimensions() const on arg 2. No
//   direct caller in the image. The catalog label "CTCActionUseReadable_SetupRenderTarget" is descriptive.
// Callees: CRenderTarget::AttachTarget 0x00A10E60, CDisplayManager::SetRenderTarget(CRenderTarget&) 0x009BF160 (bool,
//   ignored) on the display manager global 0x01436E1C, CSurface::GetDimensions() 0x009F2F90 (C2DExtentsI by hidden
//   pointer, twice), GFAYToVY 0x009E1DD0 / GFAXToVX 0x009E1DA0 (`float __fastcall (float)`, the float pushed), the render
//   manager's CRenderManagerCore::SetVWindow(const C2DBoxF&) 0x00A0B560 on the render manager global 0x01436E18 (the landed
//   CProgressDisplay::RenderProgress 0x00499880 model). 2^32 pooled at 0x0122DCB4 (the unsigned-to-float fix-up).
// Shape: one statement `SetViewport(C2DBoxF(0, 0, GFAXToVX((float)surface.GetWidth()), GFAYToVY((float)surface.GetHeight())))`
//   through CSurface's inline GetWidth() / GetHeight() (`?GetWidth@CSurface@@QBEKXZ`, unsigned long: the 2^32 fix-up),
//   each a GetDimensions() hidden-return temporary; in one full-expression the two temporaries and the box keep their
//   own frame slots (retail's 0x20 frame), where height/width locals in statements of their own let VC7.1 pack all
//   three into one slot (0x10 frame, otherwise identical code).
// Naming (NF161, 2026-11-20, q4303 / q6167): 0x00A0B560 is ?SetVWindow@CRenderManagerCore@@QAEXABVC2DBoxF@@@Z (Ego_r.pdb 0x00B74B00 + FableWin.pdb
//   0x030EB2A0; the base of CRenderManager, engine/CRenderManager.h -> CRenderManagerCore.h); the row-local CRenderManager / C2DBoxF models are
//   replaced by the shared headers. Was ?SetViewport@CRenderManager@@QAEXABVC2DBoxF@@@Z.
#define FABLE_RETAIL_C2DBOXF
#include "engine/CRenderManager.h"   // NF161 (q4303 / q6167): was a row-local CRenderManager (SetViewport) + C2DBoxF + C2DExtentsI
#include "engine/C2DExtentsI.h"
#include "engine/C2DBoxF.h"
class CSurface;

class CSurface {
public:
    C2DExtentsI GetDimensions() const;                  // 0x009F2F90
    unsigned long GetWidth() const { return GetDimensions().X; }    // inline in retail
    unsigned long GetHeight() const { return GetDimensions().Y; }   // inline in retail
};

class CRenderTarget {
public:
    void AttachTarget(const CSurface& surface);         // 0x00A10E60
};

class CDisplayManager {
public:
    bool SetRenderTarget(CRenderTarget& target);        // 0x009BF160
};

float __fastcall GFAXToVX(float x);                     // 0x009E1DA0
float __fastcall GFAYToVY(float y);                     // 0x009E1DD0

namespace NEngineInternal { extern CRenderManager* GPRenderManager; }               // 0x01436E18
namespace NEngineInternal { extern CDisplayManager* GPDisplayManager; }   // 0x01436E1C ?GPDisplayManager@NEngineInternal@@3PAVCDisplayManager@@A

class CEngineWaterRenderer {
private:
    void SetRenderTargetToSurface(CRenderTarget& target, const CSurface& surface);
};

void CEngineWaterRenderer::SetRenderTargetToSurface(CRenderTarget& target, const CSurface& surface)
{
    target.AttachTarget(surface);
    NEngineInternal::GPDisplayManager->SetRenderTarget(target);
    NEngineInternal::GPRenderManager->SetVWindow(C2DBoxF(0.0f, 0.0f, GFAXToVX((float)surface.GetWidth()), GFAYToVY((float)surface.GetHeight())));
}