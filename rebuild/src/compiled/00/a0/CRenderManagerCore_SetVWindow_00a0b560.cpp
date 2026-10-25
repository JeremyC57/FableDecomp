// CRenderManagerCore::SetVWindow(const C2DBoxF& vwindow) -- set the render window from a box in virtual screen coordinates: convert it to an absolute box (GFVBoxToABox) and hand that to the display manager's SetViewport(const C2DBoxF&)
// Leaf: ?SetVWindow@CRenderManagerCore@@QAEXABVC2DBoxF@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A0B560, thiscall `ret 4`.
// Name: Ego_r.pdb name set ?SetVWindow@CRenderManagerCore@@QAEXABVC2DBoxF@@@Z (the only CRenderManagerCore member taking a
//   C2DBoxF that converts V -> A; SetAWindow(const C2DBoxI&) / SetAWindow(const C2DBoxF&) take absolute boxes). Callers
//   build virtual-coordinate boxes: the landed CEngineWaterRenderer::SetRenderTargetToSurface 0x00B7AD20 passes
//   C2DBoxF(0, 0, GFAXToVX(width), GFAYToVY(height)), and the landed NDisplayView::CView3DBase vfunc 3 0x0069E980 already
//   declares this callee as CRenderManagerCore::SetVWindow(const C2DBoxF&). 18 call sites (xrefs), none virtual.
//   The landed name CTCInventoryBase::SetViewportFloat was propagated.
// Callees: GFVBoxToABox(const C2DBoxF&, C2DBoxF&) @ 0x009E1E70 (Ego_r.pdb ?GFVBoxToABox@@YIXABVC2DBoxF@@AAV1@@Z, __fastcall:
//   the virtual box in ecx, the absolute box in edx), the landed CDisplayManager::SetViewport(const C2DBoxF&) @ 0x009BF490
//   (which rounds to a C2DBoxI itself) on DisplayManager. The landed source modelled the absolute box as a C2DBoxI.
// Layout: engine/CRenderManagerCore.h (Ego_r) DisplayManager +0x3a3c, read through a method-only view of the class (the
//   generated header is data-only), as the landed 0x0069E980 does.
#define CRenderManagerCore CRenderManagerCorePdb
#include "engine/CRenderManagerCore.h"
#undef CRenderManagerCore

class C2DBoxF {
public:
    float left, top, right, bottom;
};

void __fastcall GFVBoxToABox(const C2DBoxF& vbox, C2DBoxF& abox);   // 0x009E1E70

struct CDisplayManager {
    void SetViewport(const C2DBoxF& viewport);       // 0x009BF490
};

struct CRenderManagerCore {                          // method-only view of engine/CRenderManagerCore.h
    void SetVWindow(const C2DBoxF& vwindow);
};

void CRenderManagerCore::SetVWindow(const C2DBoxF& vwindow)
{
    C2DBoxF awindow;
    GFVBoxToABox(vwindow, awindow);
    reinterpret_cast<CRenderManagerCorePdb*>(this)->DisplayManager->SetViewport(awindow);
}

typedef char SharedRenderManagerExtent[sizeof(CRenderManagerCorePdb) == 0x3d20 ? 1 : -1];
