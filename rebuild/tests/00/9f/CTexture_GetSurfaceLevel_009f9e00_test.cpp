#include <cstdio>
static int g_released = 0;
struct IDirect3DSurface9 { int refs; unsigned long AddRef() { return ++refs; } unsigned long Release() { ++g_released; return --refs; } };
static IDirect3DSurface9 g_levels[4];
struct IDirect3DTexture9 {
    long GetSurfaceLevel(unsigned long level, IDirect3DSurface9** out) { g_levels[level].refs++; *out = &g_levels[level]; return 0; }
};
class CSurface {
public:
    IDirect3DSurface9* PD3DSurface; unsigned long AllocationSource; void* PAllocatedMemory;
    CSurface() : PD3DSurface(0), AllocationSource(0) { PAllocatedMemory = 0; }
    CSurface(const CSurface& o) : PD3DSurface(o.PD3DSurface), AllocationSource(o.AllocationSource), PAllocatedMemory(0)
    { if (AllocationSource == 2) PAllocatedMemory = o.PAllocatedMemory; if (PD3DSurface) PD3DSurface->AddRef(); }
    virtual ~CSurface() { Uninitialise(); }
    void Uninitialise() { if (PD3DSurface) { PD3DSurface->Release(); PD3DSurface = 0; AllocationSource = 0; } }
    void InitialiseFromTextureMipmap(IDirect3DSurface9* s) { Uninitialise(); PD3DSurface = s; AllocationSource = 3; }
};
class CTexture {
public:
    IDirect3DTexture9* PD3DTexture;
    CSurface GetSurfaceLevel(unsigned long level) const
    {
        IDirect3DSurface9* d3dSurface;
        PD3DTexture->GetSurfaceLevel(level, &d3dSurface);
        CSurface surface;
        surface.InitialiseFromTextureMipmap(d3dSurface);
        return surface;
    }
};
int main()
{
    IDirect3DTexture9 d3d; CTexture t; t.PD3DTexture = &d3d;
    bool ok;
    {
        CSurface s = t.GetSurfaceLevel(2);
        ok = s.PD3DSurface == &g_levels[2] && s.AllocationSource == 3 && g_levels[2].refs >= 1;
    }
    ok = ok && g_levels[2].refs == 0 && g_released >= 1;
    if (!ok) { std::printf("009f9e00_TEST FAIL\n"); return 1; }
    std::printf("009f9e00_TEST PASS\n");
    return 0;
}