// CRenderTarget::DetachZBuffer() -- release the z-buffer surface if it holds a D3D surface.
// Leaf: ?DetachZBuffer@CRenderTarget@@QAEXXZ
// NF37 (2026-11-16): moved off the `DetachTarget(S*)` free stand-in onto the shared engine/CRenderTarget.h (Ego_r.pdb / FableWin.pdb
// ?DetachZBuffer@CRenderTarget@@QAEXXZ; Ego_r donor order DetachTarget 0x00B7EE20 / DetachZBuffer 0x00B7EE30 / Uninitialise 0x00B7EE40 =
// retail 0x00A10E10 / 0x00A10E20 / 0x00A10E30; the body tests ZBufferSurface +0x14 .PD3DSurface +0x18). Callee CSurface::Uninitialise 0x009F2E20.
#define FABLE_RETAIL_CSURFACE
#define FABLE_RETAIL_CSURFACE_GETD3DSURFACE
#include "engine/CRenderTarget.h"

void CRenderTarget::DetachZBuffer()
{
    if (ZBufferSurface.GetD3DSurface())
        ZBufferSurface.Uninitialise();
}
