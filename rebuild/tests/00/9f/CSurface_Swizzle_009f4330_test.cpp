#include <cstdio>
struct D3DSURFACE_DESC_Local { unsigned long Format, Type, Usage, Pool, MultiSampleType, MultiSampleQuality, Width, Height; };
static int g_getDescCalls = 0, g_addRefs = 0, g_releases = 0, g_unlocks = 0;
struct IDirect3DSurface9 {
    unsigned long m_format, m_width, m_height;
    long m_refs;
    virtual unsigned long AddRef() { ++g_addRefs; return ++m_refs; }
    virtual unsigned long Release() { ++g_releases; return --m_refs; }
    virtual long GetDesc(D3DSURFACE_DESC_Local* desc) { ++g_getDescCalls; desc->Format = m_format; desc->Width = m_width; desc->Height = m_height; return 0; }
    virtual long UnlockRect() { ++g_unlocks; return 0; }
    virtual ~IDirect3DSurface9() {}
};
struct CPixelFormatFromD3D {
    long FormatIndex;
    CPixelFormatFromD3D(unsigned long d3dFormat) : FormatIndex((long)d3dFormat) {}
    unsigned long GetColourDepth() const { return FormatIndex == 21 ? 32 : FormatIndex == 23 ? 16 : 8; }
    unsigned long GetByteColourDepth() const { return GetColourDepth() / 8; }
};
struct CSurface {
    void* __vftable;
    IDirect3DSurface9* PD3DSurface;
    unsigned long AllocationSource;
    void* PAllocatedMemory;
    unsigned long m_lastLockFlags;
    int m_locks;
    struct CSurfaceLockedRect { long Width, Height, Pitch; unsigned char* PMem; };
    CSurfaceLockedRect Lock(unsigned long lockFlags) { m_lastLockFlags = lockFlags; ++m_locks; CSurfaceLockedRect r; r.Width = 1; r.Height = 1; r.Pitch = 4; r.PMem = 0; return r; }
    void Swizzle(unsigned long lockFlags) { CSurfaceLockedRect rect = Lock(lockFlags); PD3DSurface->UnlockRect(); }
};
int main()
{
    IDirect3DSurface9 d3d; d3d.m_format = 21; d3d.m_width = 4; d3d.m_height = 4; d3d.m_refs = 1;
    CSurface s; s.PD3DSurface = &d3d; s.m_locks = 0; s.m_lastLockFlags = 0;
    s.Swizzle(0x10);
    if (s.m_locks != 1 || s.m_lastLockFlags != 0x10 || g_unlocks != 1) { std::printf("009f4330_TEST FAIL\n"); return 1; }
    std::printf("009f4330_TEST PASS\n");
    return 0;
}