// CSurface::SetPixel(long x, long y, CRGBColour colour, const CSurfaceLockedRect& rect) -- write one pixel into a locked surface: take the surface's pixel format (GetPixelFormat, inlined), address the pixel at PMem + y * Pitch + x * bytes-per-pixel and let the format encode the colour there
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009F31C0, length 82 bytes.
// Callees: IDirect3DSurface9::GetDesc (COM slot 12), CPixelFormat::CPixelFormat(D3DFORMAT) @ 0x009E3830,
// CPixelFormat::GetByteColourDepth @ 0x009E3800, CPixelFormat::WriteColour(colour, destination) @ 0x009E3F60.
// Shape: 0x24-byte frame = the CPixelFormat local (+0, the inlined GetPixelFormat's hidden result) and its
// D3DSURFACE_DESC (+4); `ret 10h` (x, y, colour by value, the locked rect by reference).
// The address is `PMem + y*Pitch + x*depth` with the depth a named local (retail: depth in eax, `imul eax,[x]`,
// then `add edx,eax ; add edx,[PMem]`). Tried: the same sum with the depth call inline (86 v 82: the depth copied
// to ecx, rect in eax) and a named `long offset` local added to PMem (84 v 82).
// Model: engine/CSurface.h (+4 PD3DSurface) with a methods struct; PDB CSurfaceLockedRect.
#include "engine/CSurface.h"

struct D3DSURFACE_DESC_Local {                     // D3DSURFACE_DESC, 0x20 bytes
    unsigned long Format;                          // +0x00 D3DFORMAT
    unsigned long Type;                            // +0x04
    unsigned long Usage;                           // +0x08
    unsigned long Pool;                            // +0x0c
    unsigned long MultiSampleType;                 // +0x10
    unsigned long MultiSampleQuality;              // +0x14
    unsigned long Width;                           // +0x18
    unsigned long Height;                          // +0x1c
};

// The COM surface (completes the header's forward declaration): IUnknown + IDirect3DResource9 slots.
struct IDirect3DSurface9 {
    virtual long __stdcall QueryInterface(const void* iid, void** object);   // slot 0
    virtual unsigned long __stdcall AddRef();                                // slot 1 (+0x04)
    virtual unsigned long __stdcall Release();                               // slot 2 (+0x08)
    virtual long __stdcall GetDevice(void** device);                         // slot 3
    virtual long __stdcall SetPrivateData(const void* guid, const void* data, unsigned long size, unsigned long flags);
    virtual long __stdcall GetPrivateData(const void* guid, void* data, unsigned long* size);
    virtual long __stdcall FreePrivateData(const void* guid);
    virtual unsigned long __stdcall SetPriority(unsigned long priority);
    virtual unsigned long __stdcall GetPriority();
    virtual void __stdcall PreLoad();
    virtual long __stdcall GetType();
    virtual long __stdcall GetContainer(const void* iid, void** container);  // slot 11
    virtual long __stdcall GetDesc(D3DSURFACE_DESC_Local* desc);             // slot 12 (+0x30)
    virtual long __stdcall LockRect(void* lockedRect, const void* rect, unsigned long flags);   // slot 13 (+0x34)
    virtual long __stdcall UnlockRect();                                     // slot 14 (+0x38)
};

struct CRGBColour {                                // a 4-byte colour passed by value
    unsigned char B, G, R, A;
};

struct C2DExtentsI {
    long Width;
    long Height;
};

struct CSurfaceLockedRect {                        // PDB CSurfaceLockedRect, 16 bytes
    C2DExtentsI    Dimensions;                     // +0x00
    long           Pitch;                          // +0x08
    unsigned char* PMem;                           // +0x0c
};

// CPixelFormat with its D3DFORMAT constructor and depth queries (the engine header holds only the layout).
struct CPixelFormatFromD3D {
    long FormatIndex;                              // +0x00 (PDB CPixelFormat::FormatIndex)
    CPixelFormatFromD3D(unsigned long d3dFormat);  // 0x009E3830 (CPixelFormat::Initialise / constructor)
    unsigned long GetColourDepth() const;          // 0x009E3820 (bits per pixel)
    unsigned long GetByteColourDepth() const;      // 0x009E3800 (bytes per pixel)
    void WriteColour(CRGBColour colour, unsigned char* destination) const;   // 0x009E3F60 (encode one pixel)
};

struct CSurface_Methods : CSurface {
    CPixelFormatFromD3D GetPixelFormat() const;
    void SetPixel(long x, long y, CRGBColour colour, const CSurfaceLockedRect& rect) const;
};

CPixelFormatFromD3D CSurface_Methods::GetPixelFormat() const
{
    D3DSURFACE_DESC_Local desc;
    PD3DSurface->GetDesc(&desc);
    return CPixelFormatFromD3D(desc.Format);
}

void CSurface_Methods::SetPixel(long x, long y, CRGBColour colour, const CSurfaceLockedRect& rect) const
{
    CPixelFormatFromD3D format = GetPixelFormat();
    long depth = format.GetByteColourDepth();
    format.WriteColour(colour, rect.PMem + y * rect.Pitch + x * depth);
}
// ---- harness ----
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
static unsigned long g_format = 21;
static int g_descCalls = 0;
static unsigned char* g_dest = 0; static CRGBColour g_col; static long g_writeFormat = -1;
long __stdcall IDirect3DSurface9::QueryInterface(const void*, void**) { return 0; }
unsigned long __stdcall IDirect3DSurface9::AddRef() { return 1; }
unsigned long __stdcall IDirect3DSurface9::Release() { return 0; }
long __stdcall IDirect3DSurface9::GetDevice(void**) { return 0; }
long __stdcall IDirect3DSurface9::SetPrivateData(const void*, const void*, unsigned long, unsigned long) { return 0; }
long __stdcall IDirect3DSurface9::GetPrivateData(const void*, void*, unsigned long*) { return 0; }
long __stdcall IDirect3DSurface9::FreePrivateData(const void*) { return 0; }
unsigned long __stdcall IDirect3DSurface9::SetPriority(unsigned long) { return 0; }
unsigned long __stdcall IDirect3DSurface9::GetPriority() { return 0; }
void __stdcall IDirect3DSurface9::PreLoad() { }
long __stdcall IDirect3DSurface9::GetType() { return 1; }
long __stdcall IDirect3DSurface9::GetContainer(const void*, void**) { return 0; }
long __stdcall IDirect3DSurface9::GetDesc(D3DSURFACE_DESC_Local* desc) { memset(desc, 0, sizeof *desc); desc->Format = g_format; ++g_descCalls; return 0; }
long __stdcall IDirect3DSurface9::LockRect(void*, const void*, unsigned long) { return 0; }
long __stdcall IDirect3DSurface9::UnlockRect() { return 0; }
CPixelFormatFromD3D::CPixelFormatFromD3D(unsigned long d3dFormat) { FormatIndex = (long)d3dFormat; }
unsigned long CPixelFormatFromD3D::GetByteColourDepth() const { return FormatIndex == 21 ? 4 : 2; }
void CPixelFormatFromD3D::WriteColour(CRGBColour colour, unsigned char* destination) const { g_dest = destination; g_col = colour; g_writeFormat = FormatIndex; }
int main()
{
    static unsigned char mem[64 * 32];
    IDirect3DSurface9 d3d;
    CSurface_Methods s; memset(&s, 0, sizeof s); s.PD3DSurface = &d3d;
    CSurfaceLockedRect r; memset(&r, 0, sizeof r); r.Pitch = 64; r.PMem = mem;
    CRGBColour c; c.B = 1; c.G = 2; c.R = 3; c.A = 4;
    s.SetPixel(5, 7, c, r);                     // 32-bit format: 7*64 + 5*4
    CHECK(g_descCalls == 1 && g_writeFormat == 21);
    CHECK(g_dest == mem + 7 * 64 + 5 * 4);
    CHECK(g_col.B == 1 && g_col.G == 2 && g_col.R == 3 && g_col.A == 4);
    g_format = 23;                              // 16-bit format: 3*64 + 9*2
    c.R = 0x80;
    s.SetPixel(9, 3, c, r);
    CHECK(g_descCalls == 2 && g_writeFormat == 23);
    CHECK(g_dest == mem + 3 * 64 + 9 * 2);
    CHECK(g_col.R == 0x80);
    if (fails == 0) printf("009f31c0_TEST PASS\n");
    return fails;
}