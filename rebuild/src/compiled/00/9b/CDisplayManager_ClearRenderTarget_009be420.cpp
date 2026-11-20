// CDisplayManager::ClearRenderTarget(const CRGBColour&, unsigned long) -- map the clear request bits onto D3DCLEAR flags and clear the device
// Leaf: ?ClearRenderTarget@CDisplayManager@@QAEXABVCRGBColour@@K@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x009BE420, thiscall `ret 8`. Ego_r.pdb 0x00B23BC0 / FableWin.pdb 0x0301E730
//   ?ClearRenderTarget@CDisplayManager@@QAEXABVCRGBColour@@K@Z (public). Request bit 1 -> D3DCLEAR_ZBUFFER (2); bit 2 -> D3DCLEAR_STENCIL (4)
//   only when the depth buffer's format has stencil bits; bit 4 -> D3DCLEAR_TARGET (1). The stencil test re-resolves the depth format's
//   D3D format through CPixelFormat::FindFormatInfoIndexFromD3DFormat 0x009E3620 (public static __fastcall, called at 0x009BE44A) and
//   reads GD3DFormats[].StencilBits (+0x1C, `[ecx*4+129BA5Ch]`); the device is IDirect3DDevice9::Clear (slot 43, +0xAC) on D3DDevice +0x58.
// NF136 (2026-11-20, q4290): moved off the CDisplayManagerClearRenderTargetView stand-in (and the FableFindFormatInfoIndexFromD3DFormat_009E3620 /
//   g_FablePixelFormatInfoTable_0129BA40 callee spellings) onto engine/CDisplayManager.h, engine/CPixelFormat.h and engine/CD3DFormatInfo.h.
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_CDISPLAYMANAGER
#define FABLE_RETAIL_CPIXELFORMAT
#define FABLE_RETAIL_CPIXELFORMAT_SIGNED
#define FABLE_RETAIL_CPIXELFORMAT_FINDFORMATINFOINDEX
#define FABLE_RETAIL_CRGBCOLOUR_GETD3DCOLOR
#include "engine/CRGBColour.h"
#include "engine/IDirect3DDevice9.h"
#include "engine/CDisplayManager.h"

void CDisplayManager::ClearRenderTarget(const CRGBColour& colour, unsigned long flags)
{
    unsigned long d3d_flags = 0;
    if ((flags & 1) != 0)
        d3d_flags = 2;
    if ((flags & 2) != 0)
    {
        if (GD3DFormats[CPixelFormat::FindFormatInfoIndexFromD3DFormat(DepthBufferFormat.GetAsD3DPixelFormat())].StencilBits != 0)
            d3d_flags |= 4;
    }
    if ((flags & 4) != 0)
        d3d_flags |= 1;
    D3DDevice->Clear(0, 0, d3d_flags, colour.GetD3DCOLOR(), 1.0f, 0);
}
