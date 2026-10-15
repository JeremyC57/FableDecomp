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
    CPixelFormatFromD3D GetPixelFormat() const { D3DSURFACE_DESC_Local desc; PD3DSurface->GetDesc(&desc); return CPixelFormatFromD3D(desc.Format); }
    unsigned long GetWidth() const { D3DSURFACE_DESC_Local desc; PD3DSurface->GetDesc(&desc); return desc.Width; }
    unsigned long GetPitch() const { return GetPixelFormat().GetColourDepth() * GetWidth() / 8; }
};
int main()
{
    IDirect3DSurface9 d3d; d3d.m_format = 21; d3d.m_width = 64; d3d.m_height = 32; d3d.m_refs = 1;
    CSurface s; s.PD3DSurface = &d3d;
    if (s.GetPitch() != 256 || g_getDescCalls != 2) { std::printf("009f3390_TEST FAIL\n"); return 1; }
    d3d.m_format = 7; d3d.m_width = 9;
    if (s.GetPitch() != 9) { std::printf("009f3390_TEST FAIL\n"); return 1; }
    std::printf("009f3390_TEST PASS\n");
    return 0;
}