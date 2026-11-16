// CSurfaceCompositor::CSurfaceCompositor(CSurface& dest_surface) -- CBaseClass ctor, clear the capture block, take the destination surface, create
// the compositor's own CRenderTarget into its counted pointer, zero the blend type and layer count
// Leaf: ??0CSurfaceCompositor@@QAE@AAVCSurface@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00F38990, length 117 bytes (+11 int3 padding). `ret 4`.
// Callees: CBaseClass::CBaseClass @ 0x0099A2D0 (twice: own base, then the new CRenderTarget's base), operator new @ 0x00BFEA1A (0x24 bytes),
//   CCountedPointer<CRenderTarget>::Reset(CRenderTarget*) @ 0x00F39CA0. Vtables: CSurfaceCompositor 0x01301CF0, CRenderTarget 0x0129C800,
//   CSurface 0x0122F84C.
// Shape: init list = StateCaptureBlock (its inline ctor clears the byte at +8), DestSurface, RenderTarget (two zero words); body =
//   Reset(new CRenderTarget), then CurrentBlend = 0 and LayerCount = 0 (stored last). The CRenderTarget default ctor is inline: CBaseClass,
//   vtable, two embedded CSurface (vtable shared through eax, PD3DSurface / AllocationSource zeroed).
// NF39 (2026-11-16): moved off its local CSurfaceCompositor / CRenderTarget / CSurface / capture-block models (pointer argument) onto the shared
//   engine/CSurfaceCompositor.h (which names 0x00F38990), engine/CRenderTarget.h, engine/CSurface.h, engine/CRenderStateCaptureBlock.h (inline
//   ctor) and engine/CCountedPointer.h. Evidence: Ego_r.pdb / FableWin.pdb ??0CSurfaceCompositor@@QAE@AAVCSurface@@@Z; RenderToTexture 0x00B2B180
//   passes the render surface by reference (already bound).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CSURFACE
#define FABLE_RETAIL_C2DBOXF
#define FABLE_RETAIL_CRENDERSTATECAPTUREBLOCK_INLINE_CTOR
#include "engine/Direct3D9Types.h"
#include "engine/C2DBoxF.h"
#include "engine/C3DVector.h"
#include "engine/CSurface.h"
#include "engine/CRenderTarget.h"
#include "engine/CSurfaceCompositor.h"

CSurfaceCompositor::CSurfaceCompositor(CSurface& dest_surface)
    : DestSurface(&dest_surface)
{
    RenderTarget.Reset(new CRenderTarget);
    CurrentBlend = COMPOSITE_BLEND_NULL;
    LayerCount = 0;
}
