// CDisplayManager::GetClosestDepthBufferFormat(const D3DDISPLAYMODE& mode, long bitDepth) const -- among the depth formats with at least 8 stencil bits that the device supports as a depth-stencil surface matching the mode, pick the one whose depth is closest to the request (ties go to the deeper format)
// Grade: DIFFER(236v253) -- register allocation: retail strength-reduces the table walk into TWO pointers (entry in edi, &BitDepth in ebx) and keeps i, bestDifference and best in the frame; ours uses one &BitDepth pointer and keeps bestDifference in ebx
// Leaf: GetClosestDepthBufferFormat
// Retail: Fable.exe (TLC, VC7.1) @ 0x009BEBB0, 253 bytes, `ret 0Ch` (hidden CPixelFormat result, mode, bitDepth). Called from
//   0x009BF8CC and 0x009BFC92. PDB: ?GetClosestDepthBufferFormat@CDisplayManager@@ABE?AVCPixelFormat@@ABU_D3DDISPLAYMODE@@J@Z.
// Label correction: catalogued as CPixelFormat::FindBestMatch; `this` is the CDisplayManager (D3D +0x54, caps +0x5C/+0x60).
// Callees: IDirect3D9 slots 10 CheckDeviceFormat (+0x28) and 12 CheckDepthStencilMatch (+0x30) (COM `__stdcall`, `this`
//   pushed), CPixelFormat::CPixelFormat(D3DFORMAT) 0x009E3830 (catalogued as CPixelFormat::Initialise).
// Globals: the 36-byte CD3DFormatInfo table 0x0129BA40 (BitDepth -1 terminates; PFType 3 = depth), as the landed siblings
//   GetBestOpaqueTextureFormat 0x009BE610 / IsValidRenderTargetFormat 0x009BE930.
// Shape: the siblings' indexed `for` over the table (strength-reduced to an entry pointer plus &BitDepth); the candidate is
//   filtered by type and stencil bits, then the two device checks (usage 2 = D3DUSAGE_DEPTHSTENCIL, type 1 = D3DRTYPE_SURFACE);
//   the distance is `abs()` (cdq/xor/sub); the result is constructed straight into the hidden return slot.
#include <stdlib.h>
#pragma intrinsic(abs)

struct IDirect3D9 {                                // COM: IUnknown + IDirect3D9 slots up to CheckDepthStencilMatch
    virtual long __stdcall QueryInterface(const void* iid, void** object);
    virtual unsigned long __stdcall AddRef();
    virtual unsigned long __stdcall Release();
    virtual long __stdcall RegisterSoftwareDevice(void* initializeFunction);
    virtual unsigned long __stdcall GetAdapterCount();
    virtual long __stdcall GetAdapterIdentifier(unsigned long adapter, unsigned long flags, void* identifier);
    virtual unsigned long __stdcall GetAdapterModeCount(unsigned long adapter, long format);
    virtual long __stdcall EnumAdapterModes(unsigned long adapter, long format, unsigned long mode, void* displayMode);
    virtual long __stdcall GetAdapterDisplayMode(unsigned long adapter, void* displayMode);
    virtual long __stdcall CheckDeviceType(unsigned long adapter, long devType, long adapterFormat, long backBufferFormat, int windowed);
    virtual long __stdcall CheckDeviceFormat(unsigned long adapter, long devType, long adapterFormat, unsigned long usage,
                                             long resourceType, long checkFormat);   // slot 10 (+0x28)
    virtual long __stdcall CheckDeviceMultiSampleType(unsigned long adapter, long devType, long surfaceFormat, int windowed,
                                                      long multiSampleType, unsigned long* qualityLevels);
    virtual long __stdcall CheckDepthStencilMatch(unsigned long adapter, long devType, long adapterFormat,
                                                  long renderTargetFormat, long depthStencilFormat);   // slot 12 (+0x30)
};

enum { D3DUSAGE_DEPTHSTENCIL = 2, D3DRTYPE_SURFACE = 1 };

struct _D3DDISPLAYMODE {
    unsigned long Width;
    unsigned long Height;
    unsigned long RefreshRate;
    long          Format;                          // +0x0c
};

struct CD3DFormatInfo {                            // 36-byte entry of the format table
    long D3DFormat;                                // +0x00 _D3DFORMAT
    long PFType;                                   // +0x04 EPixelFormatType (3 depth)
    long BitDepth;                                 // +0x08 (-1 terminates the table)
    long AlphaBits;                                // +0x0c
    long RBits;                                    // +0x10
    long GBits;                                    // +0x14
    long BBits;                                    // +0x18
    long StencilBits;                              // +0x1c
    bool Linear;                                   // +0x20
};

extern const CD3DFormatInfo g_D3DFormatInfo[];     // 0x0129BA40

struct CPixelFormat {
    long FormatIndex;                              // +0
    CPixelFormat(long d3dFormat);                  // 0x009E3830
};

struct CDisplayManager {
    unsigned char _pad[0x54];                      // vptr slot, SystemManager, PRenderManager, surfaces, back-buffer copy texture
    IDirect3D9*   D3D;                             // +0x54
    void*         D3DDevice;                       // +0x58
    long          DeviceType;                      // +0x5c D3DDeviceCaps.DeviceType
    unsigned long AdapterOrdinal;                  // +0x60 D3DDeviceCaps.AdapterOrdinal
    CPixelFormat GetClosestDepthBufferFormat(const _D3DDISPLAYMODE& mode, long bitDepth) const;
};

CPixelFormat CDisplayManager::GetClosestDepthBufferFormat(const _D3DDISPLAYMODE& mode, long bitDepth) const
{
    long best = -1;
    long bestDifference = 0x7fffffff;
    for (long i = 0; g_D3DFormatInfo[i].BitDepth != -1; ++i) {
        const CD3DFormatInfo& info = g_D3DFormatInfo[i];
        if (info.PFType == 3 && info.StencilBits >= 8
            && D3D->CheckDeviceFormat(AdapterOrdinal, DeviceType, mode.Format, D3DUSAGE_DEPTHSTENCIL, D3DRTYPE_SURFACE, info.D3DFormat) >= 0
            && D3D->CheckDepthStencilMatch(AdapterOrdinal, DeviceType, mode.Format, mode.Format, info.D3DFormat) >= 0) {
            long difference = abs(bitDepth - info.BitDepth);
            if (difference < bestDifference) {
                bestDifference = difference;
                best = i;
            }
            else if (difference == bestDifference && bitDepth < info.BitDepth) {
                bestDifference = difference;
                best = i;
            }
        }
    }
    return CPixelFormat(g_D3DFormatInfo[best].D3DFormat);
}
