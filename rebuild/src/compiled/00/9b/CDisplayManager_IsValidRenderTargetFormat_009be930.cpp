// CDisplayManager::IsValidRenderTargetFormat(const CPixelFormat&, const CPixelFormat&) -- ask Direct3D whether the render-target / depth-stencil pair matches the current mode
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009BE930, length 80 bytes.
// Callees: IDirect3D9 vslot 12 CheckDepthStencilMatch(Adapter, DeviceType, AdapterFormat, RenderTargetFormat, DepthStencilFormat)
// on D3D (+0x54). The D3DFORMAT of each pixel format comes from the 36-byte entries of the format table @ 0x0129BA40
// indexed by FormatIndex (`lea edx,[eax+eax*8]` then `[edx*4+table]`).
// PDB CDisplayManager: D3D +84, D3DDeviceCaps +92 (D3DCAPS9: DeviceType +0 = +0x5c, AdapterOrdinal +4 = +0x60),
// CurrentMode +440 (D3DDISPLAYMODE: Format +12 = +0x1c4). The arguments are evaluated right to left (depth-stencil
// first) through an inline table accessor (a direct subscript in the call or named locals loads the D3D interface and
// its vtable before the formats; the accessor keeps them after, with the render-target format parked in esi), and the
// result is `SUCCEEDED(hr)` as a bool through a full register (`setge dl ; mov al,dl`).
// IDirect3D9: the shared header (d3d9.h slot order and SDK parameter types; engine/IDirect3D9.h).
#include "engine/IDirect3D9.h"














class CPixelFormat { public:
    long FormatIndex;                  // +0
};

#include "engine/CD3DFormatInfo.h"   // CD3DFormatInfo + GD3DFormats 0x0129BA40 (name-fix NF29, both PDBs ?GD3DFormats@@3QBVCD3DFormatInfo@@B: was the local SPixelFormatInfo table g_PixelFormatTable)


class CDisplayManager { public:
    unsigned char _pad[0x54];          // +0x00 vptr, SystemManager, PRenderManager, surfaces, back-buffer copy texture
    IDirect3D9*   D3D;                 // +0x54
    void*         D3DDevice;           // +0x58
    D3DDEVTYPE    D3DDeviceCaps_DeviceType;       // +0x5c D3DCAPS9::DeviceType
    unsigned long D3DDeviceCaps_AdapterOrdinal;   // +0x60 D3DCAPS9::AdapterOrdinal
    unsigned char _caps[0x1b8 - 0x64]; // +0x64 the rest of D3DCAPS9, dimensions, flags, formats, window handle
    unsigned long CurrentMode_Width;   // +0x1b8 D3DDISPLAYMODE
    unsigned long CurrentMode_Height;  // +0x1bc
    unsigned long CurrentMode_RefreshRate;   // +0x1c0
    D3DFORMAT     CurrentMode_Format;  // +0x1c4
    bool IsValidRenderTargetFormat(const CPixelFormat& renderTargetFormat, const CPixelFormat& depthStencilFormat);
};

static inline D3DFORMAT GetD3DFormat(const CPixelFormat& format)
{
    return GD3DFormats[format.FormatIndex].D3DFormat;
}

bool CDisplayManager::IsValidRenderTargetFormat(const CPixelFormat& renderTargetFormat, const CPixelFormat& depthStencilFormat)
{
    return D3D->CheckDepthStencilMatch(D3DDeviceCaps_AdapterOrdinal, D3DDeviceCaps_DeviceType, CurrentMode_Format,
                                       GetD3DFormat(renderTargetFormat), GetD3DFormat(depthStencilFormat)) >= 0;
}