// Behaviour test for 009bebb0: src/009bebb0.cpp copied verbatim, the format table and the device stubbed below.
#include <cstdio>
#include <cstring>
static int fails = 0;
static char g_msg[128];
#define CHECK(c) do { if (!(c)) { ++fails; _snprintf(g_msg, sizeof(g_msg), "FAIL %d: %s", __LINE__, #c); puts(g_msg); } } while (0)
// ---- source (verbatim) ----
// CDisplayManager::GetClosestDepthBufferFormat(const D3DDISPLAYMODE& mode, long bitDepth) const -- among the depth formats with at least 8 stencil bits that the device supports as a depth-stencil surface matching the mode, pick the one whose depth is closest to the request (ties go to the deeper format)
// Leaf: ?GetClosestDepthBufferFormat@CDisplayManager@@ABE?AVCPixelFormat@@ABU_D3DDISPLAYMODE@@J@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x009BEBB0, 253 bytes, `ret 0Ch` (hidden CPixelFormat result, mode, bitDepth). Called from
//   0x009BF8CC and 0x009BFC92. PDB: ?GetClosestDepthBufferFormat@CDisplayManager@@ABE?AVCPixelFormat@@ABU_D3DDISPLAYMODE@@J@Z.
// Label correction: catalogued as CPixelFormat::FindBestMatch; `this` is the CDisplayManager (D3D +0x54, caps +0x5C/+0x60).
// Callees: IDirect3D9 slots 10 CheckDeviceFormat (+0x28) and 12 CheckDepthStencilMatch (+0x30) (COM `__stdcall`, `this`
//   pushed), CPixelFormat::CPixelFormat(D3DFORMAT) 0x009E3830 (catalogued as CPixelFormat::Initialise).
// Globals: the 36-byte CD3DFormatInfo table 0x0129BA40 (BitDepth -1 terminates; PFType 3 = depth), as the landed siblings
//   GetBestOpaqueTextureFormat 0x009BE610 / IsValidRenderTargetFormat 0x009BE930.
// Shape: the siblings' indexed `for` over the table; the type, stencil and format reads go through the `info` reference
//   but the depth reads re-index `g_D3DFormatInfo[i].BitDepth`, which VC7.1 shares with the loop test as a second pointer
//   (&BitDepth in ebx beside the entry in edi, so i, bestDifference and best live in the frame); the two device checks use
//   usage 2 = D3DUSAGE_DEPTHSTENCIL, type 1 = D3DRTYPE_SURFACE; the distance is `abs()` (cdq/xor/sub); the result is
//   constructed straight into the hidden return slot.
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

class CPixelFormat { public:
    long FormatIndex;                              // +0
    CPixelFormat(long d3dFormat);                  // 0x009E3830
};

class CDisplayManager { public:
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
            long difference = abs(bitDepth - g_D3DFormatInfo[i].BitDepth);
            if (difference < bestDifference) {
                bestDifference = difference;
                best = i;
            }
            else if (difference == bestDifference && bitDepth < g_D3DFormatInfo[i].BitDepth) {
                bestDifference = difference;
                best = i;
            }
        }
    }
    return CPixelFormat(g_D3DFormatInfo[best].D3DFormat);
}

// ---- stubs and driver ----
extern const CD3DFormatInfo g_D3DFormatInfo[] = {
    { 100, 2, 32, 8, 8, 8, 8, 0, false },   // 0: colour format, ignored
    { 101, 3, 16, 0, 0, 0, 0, 0, false },   // 1: depth, no stencil
    { 102, 3, 24, 0, 0, 0, 0, 8, false },   // 2: D24S8
    { 103, 3, 32, 0, 0, 0, 0, 8, false },   // 3: D32S8 (device rejects format check)
    { 104, 3, 16, 0, 0, 0, 0, 8, false },   // 4: D16S8 (device rejects stencil match)
    { 105, 3, 28, 0, 0, 0, 0, 8, false },   // 5: D28S8
    { 106, 3, 20, 0, 0, 0, 0, 8, false },   // 6: D20S8
    { 0, 0, -1, 0, 0, 0, 0, 0, false },
};
static long g_lastFormat = 0;
static int g_formatChecks = 0, g_matchChecks = 0;
CPixelFormat::CPixelFormat(long d3dFormat) { FormatIndex = d3dFormat; g_lastFormat = d3dFormat; }
long __stdcall IDirect3D9::QueryInterface(const void*, void**) { return 0; }
unsigned long __stdcall IDirect3D9::AddRef() { return 0; }
unsigned long __stdcall IDirect3D9::Release() { return 0; }
long __stdcall IDirect3D9::RegisterSoftwareDevice(void*) { return 0; }
unsigned long __stdcall IDirect3D9::GetAdapterCount() { return 0; }
long __stdcall IDirect3D9::GetAdapterIdentifier(unsigned long, unsigned long, void*) { return 0; }
unsigned long __stdcall IDirect3D9::GetAdapterModeCount(unsigned long, long) { return 0; }
long __stdcall IDirect3D9::EnumAdapterModes(unsigned long, long, unsigned long, void*) { return 0; }
long __stdcall IDirect3D9::GetAdapterDisplayMode(unsigned long, void*) { return 0; }
long __stdcall IDirect3D9::CheckDeviceType(unsigned long, long, long, long, int) { return 0; }
long __stdcall IDirect3D9::CheckDeviceFormat(unsigned long adapter, long devType, long adapterFormat, unsigned long usage,
                                             long resourceType, long checkFormat)
{
    ++g_formatChecks;
    CHECK(adapter == 7 && devType == 1 && adapterFormat == 22 && usage == 2 && resourceType == 1);
    return checkFormat == 103 ? -1 : 0;
}
long __stdcall IDirect3D9::CheckDeviceMultiSampleType(unsigned long, long, long, int, long, unsigned long*) { return 0; }
long __stdcall IDirect3D9::CheckDepthStencilMatch(unsigned long adapter, long devType, long adapterFormat,
                                                  long renderTargetFormat, long depthStencilFormat)
{
    ++g_matchChecks;
    CHECK(adapter == 7 && devType == 1 && adapterFormat == 22 && renderTargetFormat == 22);
    return depthStencilFormat == 104 ? -1 : 0;
}
int main()
{
    static IDirect3D9 d3d;
    static CDisplayManager dm;
    dm.D3D = &d3d; dm.DeviceType = 1; dm.AdapterOrdinal = 7;
    _D3DDISPLAYMODE mode; mode.Width = 640; mode.Height = 480; mode.RefreshRate = 60; mode.Format = 22;
    // 1) 24 requested: exact D24S8
    CPixelFormat r = dm.GetClosestDepthBufferFormat(mode, 24);
    CHECK(r.FormatIndex == 102 && g_lastFormat == 102);
    CHECK(g_formatChecks == 5 && g_matchChecks == 4);   // entries 2..6 reach the device; 3 fails the format check
    // 2) 16 requested: D16S8 rejected, D20S8 (distance 4) beats D24S8 (8)
    r = dm.GetClosestDepthBufferFormat(mode, 16);
    CHECK(r.FormatIndex == 106);
    // 3) 26 requested: D24S8 and D28S8 tie at 2, the deeper wins
    r = dm.GetClosestDepthBufferFormat(mode, 26);
    CHECK(r.FormatIndex == 105);
    // 4) 22 requested: D20S8 and D24S8 tie at 2, the deeper (24, found first) stays
    r = dm.GetClosestDepthBufferFormat(mode, 22);
    CHECK(r.FormatIndex == 102);
    // 5) 64 requested: the deepest valid one (D28S8; D32S8 is rejected)
    r = dm.GetClosestDepthBufferFormat(mode, 64);
    CHECK(r.FormatIndex == 105);
    if (fails == 0) printf("009bebb0_TEST PASS\n");
    return fails;
}