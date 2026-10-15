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
    void InitialiseFromPreallocatedSurface(IDirect3DSurface9* surface, bool addRef)
    {
        Uninitialise();
        PD3DSurface = surface;
        AllocationSource = surface != 0;
        if (addRef)
            surface->AddRef();
    }
};
int main()
{
    IDirect3DSurface9 oldD3d; oldD3d.m_refs = 1;
    IDirect3DSurface9 newD3d; newD3d.m_refs = 1;
    CSurface a; a.PD3DSurface = &oldD3d; a.AllocationSource = 2;
    a.InitialiseFromPreallocatedSurface(&newD3d, true);
    if (a.PD3DSurface != &newD3d || a.AllocationSource != 1 || oldD3d.m_refs != 0 || newD3d.m_refs != 2) { std::printf("009f2ec0_TEST FAIL\n"); return 1; }
    a.InitialiseFromPreallocatedSurface(0, false);
    if (a.PD3DSurface != 0 || a.AllocationSource != 0 || newD3d.m_refs != 1 || g_addRefs != 1 || g_releases != 2) { std::printf("009f2ec0_TEST FAIL\n"); return 1; }
    std::printf("009f2ec0_TEST PASS\n");
    return 0;
}