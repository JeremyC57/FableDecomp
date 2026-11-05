// CDisplayManager::SetViewport(const C2DBoxI& viewport) -- clamp the box to the render target, set the D3D viewport and notify
// Grade: DIFFER(477v479) -- dl=3: one compare; retail tests the right edge with `inc eax ; cmp eax,ebp ; jge` (ebp = the zero top register) where VC7.1 folds this spelling's `brx < 0` into the inc flags (`inc eax ; jns`); everything else (frame, register map, spills, homes) matches
// Leaf: ?SetViewport@CDisplayManager@@QAEXABVC2DBoxI@@@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x009BEF80, 479 bytes, `ret 4` (aligned entry: default /O2 TU). Ego_r.pdb / FableWin.pdb
//   ?SetViewport@CDisplayManager@@QAEXABVC2DBoxI@@@Z.
// Callees: IDirect3DDevice9::SetViewport (slot 47, +0xBC), CRenderManager OnViewportChanged 0x00A0AAC0 through the boost::scoped_ptr bool test
//   (0x009C1220 get), CShaderRenderManager::Get().OnPostViewportChanged 0x009880D0 on 0x013BC470.
// Headers (chunk 774 overlay, already landed): CDisplayManager.h FABLE_RETAIL_CDISPLAYMANAGER_SETVIEWPORT, boost/scoped_ptr.h
//   FABLE_RETAIL_SCOPED_PTR_BOOL, CRenderManager.h FABLE_RETAIL_CRENDERMANAGER_ONVIEWPORTCHANGED.
// Shape (chunk 776, from dl=138): each edge is a local copy of the viewport coordinate clamped in place inside the else of the `< 0`
//   test (`long tlx = viewport.TLX; long left; if (tlx < 0) left = 0; else { if (tlx > width) tlx = width; left = tlx; }`), the box member
//   assigned right after its edge. That gives retail's frame 0x18, the zero in ebp (top's register), width/height in edi/ebx, the viewport
//   pointer in ecx, `right` homed in the dead argument slot and the separate left/top homes. The right edge is the inner ternary
//   (retail's `mov [right],edi ; jg ; mov [right],eax`).
// Next idea: a right-edge spelling whose `< 0` test VC7.1 emits as `cmp eax,<zero reg>` instead of folding it into the `inc` flags while
//   `right` stays memory-homed (a CSE'd `viewport.BRX + 1 < 0` gives the cmp but moves `right` into ebp and drops the zero register).
#define FABLE_RETAIL_CDISPLAYMANAGER
#define FABLE_RETAIL_CDISPLAYMANAGER_SETVIEWPORT
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_SCOPED_PTR_BOOL
#define FABLE_RETAIL_CRENDERMANAGER_ONVIEWPORTCHANGED
#include "stlport/fable_containers.h"
#include "engine/Direct3D9Types.h"
#include "engine/IDirect3DDevice9.h"
#include "engine/CDisplayManager.h"
#include "engine/CRenderManager.h"
#include "engine/CShaderRenderManager.h"

void CDisplayManager::SetViewport(const C2DBoxI& viewport)
{
    long height = RenderTargetDimensions.Y;
    long width = RenderTargetDimensions.X;
    C2DBoxI box;
    long tlx = viewport.TLX;
    long left;
    if (tlx < 0) left = 0;
    else {
        if (tlx > width) tlx = width;
        left = tlx;
    }
    box.TLX = left;
    long brx = viewport.BRX + 1;
    long right;
    if (brx < 0) right = 0;
    else right = brx > width ? width : brx;
    box.BRX = right;
    long tly = viewport.TLY;
    long top;
    if (tly < 0) top = 0;
    else {
        if (tly > height) tly = height;
        top = tly;
    }
    box.TLY = top;
    long bry = viewport.BRY + 1;
    long bottom;
    if (bry < 0) bottom = 0;
    else {
        if (bry > height) bry = height;
        bottom = bry;
    }
    box.BRY = bottom;
    if (left == ViewportBox.TLX && top == ViewportBox.TLY && right == ViewportBox.BRX && bottom == ViewportBox.BRY)
        return;
    ViewportOffscreen = false;
    if (left == 0 && right == 0) {
        box.BRX = 1;
        ViewportOffscreen = true;
    } else if (left == RenderTargetDimensions.X && right == RenderTargetDimensions.X) {
        box.TLX = RenderTargetDimensions.X - 1;
        ViewportOffscreen = true;
    }
    if (top == 0 && bottom == 0) {
        box.BRY = 1;
        ViewportOffscreen = true;
    } else if (top == RenderTargetDimensions.Y && bottom == RenderTargetDimensions.Y) {
        box.TLY = RenderTargetDimensions.Y - 1;
        ViewportOffscreen = true;
    }
    D3DViewport.X = box.TLX;
    D3DViewport.Y = box.TLY;
    D3DViewport.Width = box.BRX - box.TLX;
    D3DViewport.Height = box.BRY - box.TLY;
    D3DViewport.MinZ = 0.0f;
    D3DViewport.MaxZ = 1.0f;
    if (D3DDevice->SetViewport(&D3DViewport) >= 0) {
        ViewportBox = box;
        ViewportBoxFloat.TLX = (float)box.TLX;
        ViewportBoxFloat.TLY = (float)box.TLY;
        ViewportBoxFloat.BRX = (float)box.BRX;
        ViewportBoxFloat.BRY = (float)box.BRY;
    }
    if (PRenderManager)
        PRenderManager->OnViewportChanged();
    CShaderRenderManager::Get().OnPostViewportChanged();

}
