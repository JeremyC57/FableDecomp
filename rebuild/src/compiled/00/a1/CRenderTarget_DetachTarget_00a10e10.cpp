// CRenderTarget::DetachTarget() -- release the render-to surface if it holds a D3D surface.
// Leaf: ?DetachTarget@CRenderTarget@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A10E10 (`mov eax,[ecx+8] ; test eax,eax ; je ; add ecx,4 ; jmp 9F2E20h ; ret`).
// NF142 (2026-11-20, q6305): moved off the `DetachTarget(S*)` free stand-in (which bound 0x009F2E20 as ?sub@@YIXPAUSub@@@Z) onto the shared
//   engine/CRenderTarget.h, as NF37 moved its twin DetachZBuffer 0x00A10E20: Ego_r.pdb 0x00B7EE20 + FableWin.pdb 0x030F1F10
//   ?DetachTarget@CRenderTarget@@QAEXXZ (Ego_r donor order DetachTarget / DetachZBuffer / Uninitialise = retail 0x00A10E10 / 0x00A10E20 /
//   0x00A10E30), the spelling all five landed callers already bind (0x009FAF70, 0x00B665E0, 0x00B72180, 0x00B8B780, 0x00F38930). The body
//   tests RenderToSurface +0x04 .PD3DSurface +0x08; callee ?Uninitialise@CSurface@@QAEXXZ 0x009F2E20 (Ego_r.pdb 0x00B46AF0 + FableWin.pdb 0x030C99C0).
#define FABLE_RETAIL_CSURFACE
#define FABLE_RETAIL_CSURFACE_GETD3DSURFACE
#include "engine/CRenderTarget.h"

void CRenderTarget::DetachTarget()
{
    if (RenderToSurface.GetD3DSurface())
        RenderToSurface.Uninitialise();
}
