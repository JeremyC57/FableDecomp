#include <cstdio>
static int g_addRefs = 0, g_releases = 0;
struct IDirect3DSurface9 {
    long m_refs;
    virtual unsigned long AddRef() { ++g_addRefs; return ++m_refs; }
    virtual unsigned long Release() { ++g_releases; return --m_refs; }
    virtual ~IDirect3DSurface9() {}
};
class CSurface {
public:
    virtual ~CSurface() {}
    IDirect3DSurface9* PD3DSurface;
    unsigned long      AllocationSource;
    void*              PAllocatedMemory;
    CSurface() : PD3DSurface(0), AllocationSource(0), PAllocatedMemory(0) {}
    void Uninitialise()
    {
        if (PD3DSurface) {
            PD3DSurface->Release();
            PD3DSurface = 0;
            AllocationSource = 0;
        }
    }
    void InitialiseFromTextureMipmap(IDirect3DSurface9* mipmapSurface)
    {
        Uninitialise();
        PD3DSurface = mipmapSurface;
        AllocationSource = 3;
    }
};
int main()
{
    IDirect3DSurface9 oldD3d; oldD3d.m_refs = 1;
    IDirect3DSurface9 mip; mip.m_refs = 1;
    CSurface a; a.PD3DSurface = &oldD3d; a.AllocationSource = 1;
    a.InitialiseFromTextureMipmap(&mip);
    if (a.PD3DSurface != &mip || a.AllocationSource != 3 || oldD3d.m_refs != 0 || mip.m_refs != 1 || g_addRefs != 0) { std::printf("009f2f10_TEST FAIL\n"); return 1; }
    CSurface b;
    b.InitialiseFromTextureMipmap(&mip);
    if (b.PD3DSurface != &mip || b.AllocationSource != 3 || g_releases != 1) { std::printf("009f2f10_TEST FAIL\n"); return 1; }
    std::printf("009f2f10_TEST PASS\n");
    return 0;
}
