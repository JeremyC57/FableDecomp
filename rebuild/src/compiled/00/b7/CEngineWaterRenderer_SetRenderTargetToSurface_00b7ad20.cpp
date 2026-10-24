// CEngineWaterRenderer::SetRenderTargetToSurface(CRenderTarget& target, const CSurface& surface) -- attach the surface to the render target, make it the display manager's render target, and set a full-surface viewport in virtual coordinates
// Leaf: ?SetRenderTargetToSurface@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B7AD20, 172 bytes. __fastcall (this unused), `ret 8`.
// Identity: FableWin.pdb `?SetRenderTargetToSurface@CEngineWaterRenderer@@AAEXAAVCRenderTarget@@ABVCSurface@@@Z`; the
//   body calls CRenderTarget::AttachTarget(const CSurface&) on arg 1 and CSurface::GetDimensions() const on arg 2. No
//   direct caller in the image. The catalog label "CTCActionUseReadable_SetupRenderTarget" is descriptive.
// Callees: CRenderTarget::AttachTarget 0x00A10E60, CDisplayManager::SetRenderTarget(CRenderTarget&) 0x009BF160 (bool,
//   ignored) on the display manager global 0x01436E1C, CSurface::GetDimensions() 0x009F2F90 (C2DExtentsI by hidden
//   pointer, twice), GFAYToVY 0x009E1DD0 / GFAXToVX 0x009E1DA0 (`float __fastcall (float)`, the float pushed), the render
//   manager's SetViewport(const C2DBoxF&) 0x00A0B560 on the render manager global 0x01436E18 (the landed
//   CProgressDisplay::RenderProgress 0x00499880 model). 2^32 pooled at 0x0122DCB4 (the unsigned-to-float fix-up).
// Shape: one statement `SetViewport(C2DBoxF(0, 0, GFAXToVX((float)surface.GetWidth()), GFAYToVY((float)surface.GetHeight())))`
//   through CSurface's inline GetWidth() / GetHeight() (`?GetWidth@CSurface@@QBEKXZ`, unsigned long: the 2^32 fix-up),
//   each a GetDimensions() hidden-return temporary; in one full-expression the two temporaries and the box keep their
//   own frame slots (retail's 0x20 frame), where height/width locals in statements of their own let VC7.1 pack all
//   three into one slot (0x10 frame, otherwise identical code).
class CSurface;

class C2DExtentsI {
public:
    long X;                                             // +0x00
    long Y;                                             // +0x04
    C2DExtentsI() { }
};

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

class C2DBoxF {
public:
    float X0, Y0, X1, Y1;
    C2DBoxF(float x0, float y0, float x1, float y1) : X0(x0), Y0(y0), X1(x1), Y1(y1) { }
};

class CRenderManager {
public:
    void SetViewport(const C2DBoxF& box);               // 0x00A0B560
};

float __fastcall GFAXToVX(float x);                     // 0x009E1DA0
float __fastcall GFAYToVY(float y);                     // 0x009E1DD0

extern CRenderManager*  g_pRenderManager;               // 0x01436E18
extern CDisplayManager* g_pDisplayManager;              // 0x01436E1C

class CEngineWaterRenderer {
private:
    void SetRenderTargetToSurface(CRenderTarget& target, const CSurface& surface);
};

void CEngineWaterRenderer::SetRenderTargetToSurface(CRenderTarget& target, const CSurface& surface)
{
    target.AttachTarget(surface);
    g_pDisplayManager->SetRenderTarget(target);
    g_pRenderManager->SetViewport(C2DBoxF(0.0f, 0.0f, GFAXToVX((float)surface.GetWidth()), GFAYToVY((float)surface.GetHeight())));
}