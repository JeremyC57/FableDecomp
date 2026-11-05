// CDisplayManager::SetViewport(const C2DBoxI& viewport) -- clamp the box to the render target, set the D3D viewport and notify
// Grade: DIFFER(479v479) -- register allocation: retail keeps 0 in ebp (later top), width/height in edi/ebx and the viewport pointer in ecx, spilling right to the dead argument slot; 30 spellings (8 in asm round 2b + 22 here) never give that assignment
// Leaf: ?SetViewport@CDisplayManager@@QAEXABVC2DBoxI@@@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x009BEF80, 479 bytes, `ret 4` (aligned entry: /O2). Ego_r.pdb / FableWin.pdb ?SetViewport@CDisplayManager@@QAEXABVC2DBoxI@@@Z.
// Callees: IDirect3DDevice9::SetViewport (slot 47, +0xBC), CRenderManager OnViewportChanged 0x00A0AAC0 through the boost::scoped_ptr bool test
//   (0x009C1220 get), CShaderRenderManager::Get().OnPostViewportChanged 0x009880D0 on 0x013BC470.
// Headers (chunk 774 overlay, from asm round 2b): CDisplayManager.h FABLE_RETAIL_CDISPLAYMANAGER_SETVIEWPORT, boost/scoped_ptr.h
//   FABLE_RETAIL_SCOPED_PTR_BOOL, CRenderManager.h FABLE_RETAIL_CRENDERMANAGER_ONVIEWPORTCHANGED.
// Next idea: an inline clamp helper whose minimum parameter survives inlining as a register temp (retail compares left/right/top
//   against ebp = 0 but bottom with jns), or the top-initialised-to-zero form combined with the width/height load order of retail.
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
    long left = 0, right = 0, top = 0, bottom = 0;
    if (viewport.TLX >= 0) left = viewport.TLX > width ? width : viewport.TLX;
    if (viewport.BRX + 1 >= 0) right = viewport.BRX + 1 > width ? width : viewport.BRX + 1;
    if (viewport.TLY >= 0) top = viewport.TLY > height ? height : viewport.TLY;
    if (viewport.BRY + 1 >= 0) bottom = viewport.BRY + 1 > height ? height : viewport.BRY + 1;
    C2DBoxI box;
    box.Initialise(left, top, right, bottom);
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
