// CDisplayManager::GetClosestDepthBufferFormat(const D3DDISPLAYMODE& mode, long bitDepth) const -- among the depth formats with at least 8 stencil bits that the device supports as a depth-stencil surface matching the mode, pick the one whose depth is closest to the request (ties go to the deeper format)
// Leaf: ?GetClosestDepthBufferFormat@CDisplayManager@@ABE?AVCPixelFormat@@ABU_D3DDISPLAYMODE@@J@Z
// Naming wave 25: CPixelFormat::Initialise 0x009E3830 is ?Initialise@CPixelFormat@@QAEXW4_D3DFORMAT@@@Z (Ego_r.pdb; no constructor:
//   the body returns nothing), the format a _D3DFORMAT; a CPixelFormat built from a format is the inline constructor calling it.
// Retail: Fable.exe (TLC, VC7.1) @ 0x009BEBB0, 253 bytes, `ret 0Ch` (hidden CPixelFormat result, mode, bitDepth). Called from
//   0x009BF8CC and 0x009BFC92. PDB: ?GetClosestDepthBufferFormat@CDisplayManager@@ABE?AVCPixelFormat@@ABU_D3DDISPLAYMODE@@J@Z.
// Label correction: catalogued as CPixelFormat::FindBestMatch; `this` is the CDisplayManager (D3D +0x54, caps +0x5C/+0x60).
// Callees: IDirect3D9 slots 10 CheckDeviceFormat (+0x28) and 12 CheckDepthStencilMatch (+0x30) (COM `__stdcall`, `this`
//   pushed), CPixelFormat::CPixelFormat(D3DFORMAT) 0x009E3830 (catalogued as CPixelFormat::Initialise).
// Globals: the 36-byte CD3DFormatInfo table 0x0129BA40 (BitDepth -1 terminates; PFType 3 = depth), as the landed siblings
//   GetBestOpaqueTextureFormat 0x009BE610 / IsValidRenderTargetFormat 0x009BE930.
// Shape: the siblings' indexed `for` over the table; the type, stencil and format reads go through the `info` reference
//   but the depth reads re-index `GD3DFormats[i].BitDepth`, which VC7.1 shares with the loop test as a second pointer
//   (&BitDepth in ebx beside the entry in edi, so i, bestDifference and best live in the frame); the two device checks use
//   usage 2 = D3DUSAGE_DEPTHSTENCIL, type 1 = D3DRTYPE_SURFACE; the distance is `abs()` (cdq/xor/sub); the result is
//   constructed straight into the hidden return slot.
#include <stdlib.h>
#pragma intrinsic(abs)

// IDirect3D9: the shared header (d3d9.h slot order and SDK parameter types; engine/IDirect3D9.h).
#include "engine/IDirect3D9.h"

















enum { D3DUSAGE_DEPTHSTENCIL = 2, D3DRTYPE_SURFACE = 1 };

// _D3DDISPLAYMODE {Width, Height, RefreshRate, Format +0x0c}: engine/Direct3D9Types.h (via CD3DFormatInfo.h).
#include "engine/CD3DFormatInfo.h"   // CD3DFormatInfo + GD3DFormats 0x0129BA40 (NF28: was a local struct model and g_D3DFormatInfo)


class CPixelFormat { public:
    long FormatIndex;                              // +0
    CPixelFormat(_D3DFORMAT d3dFormat) { Initialise(d3dFormat); }
    void Initialise(_D3DFORMAT d3dFormat);                  // 0x009E3830
};

class CDisplayManager { public:
    unsigned char _pad[0x54];                      // vptr slot, SystemManager, PRenderManager, surfaces, back-buffer copy texture
    IDirect3D9*   D3D;                             // +0x54
    void*         D3DDevice;                       // +0x58
    D3DDEVTYPE    DeviceType;                      // +0x5c D3DDeviceCaps.DeviceType
    unsigned long AdapterOrdinal;                  // +0x60 D3DDeviceCaps.AdapterOrdinal
private:   // name-fix pass (2026-11-12): private in both PDBs (ABE), so the compiled symbol matches the Leaf line (was public: QBE)
    CPixelFormat GetClosestDepthBufferFormat(const _D3DDISPLAYMODE& mode, long bitDepth) const;
};

CPixelFormat CDisplayManager::GetClosestDepthBufferFormat(const _D3DDISPLAYMODE& mode, long bitDepth) const
{
    long best = -1;
    long bestDifference = 0x7fffffff;
    for (long i = 0; GD3DFormats[i].BitDepth != -1; ++i) {
        const CD3DFormatInfo& info = GD3DFormats[i];
        if (info.PFType == 3 && info.StencilBits >= 8
            && D3D->CheckDeviceFormat(AdapterOrdinal, DeviceType, mode.Format, D3DUSAGE_DEPTHSTENCIL, D3DRTYPE_SURFACE, info.D3DFormat) >= 0
            && D3D->CheckDepthStencilMatch(AdapterOrdinal, DeviceType, mode.Format, mode.Format, info.D3DFormat) >= 0) {
            long difference = abs(bitDepth - GD3DFormats[i].BitDepth);
            if (difference < bestDifference) {
                bestDifference = difference;
                best = i;
            }
            else if (difference == bestDifference && bitDepth < GD3DFormats[i].BitDepth) {
                bestDifference = difference;
                best = i;
            }
        }
    }
    return CPixelFormat(GD3DFormats[best].D3DFormat);
}