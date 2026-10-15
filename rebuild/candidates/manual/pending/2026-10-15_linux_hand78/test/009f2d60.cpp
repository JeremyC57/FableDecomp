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
    CSurface(const CSurface& other)
    {
        PD3DSurface = other.PD3DSurface;
        AllocationSource = other.AllocationSource;
        if (AllocationSource == 2)
            PAllocatedMemory = other.PAllocatedMemory;
        if (PD3DSurface)
            PD3DSurface->AddRef();
    }
};
int main()
{
    IDirect3DSurface9 d3d; d3d.m_refs = 1;
    int memory = 0;
    CSurface a; a.PD3DSurface = &d3d; a.AllocationSource = 2; a.PAllocatedMemory = &memory;
    CSurface b(a);
    if (b.PD3DSurface != &d3d || b.AllocationSource != 2 || b.PAllocatedMemory != &memory || d3d.m_refs != 2 || g_addRefs != 1) { std::printf("009f2d60_TEST FAIL\n"); return 1; }
    CSurface c; c.AllocationSource = 1; c.PAllocatedMemory = &memory;
    CSurface d(c);
    if (d.PD3DSurface != 0 || d.AllocationSource != 1 || g_addRefs != 1) { std::printf("009f2d60_TEST FAIL\n"); return 1; }
    std::printf("009f2d60_TEST PASS\n");
    return 0;
}
