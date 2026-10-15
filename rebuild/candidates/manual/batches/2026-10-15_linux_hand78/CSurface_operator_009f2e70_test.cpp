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
    CSurface& operator=(const CSurface& other)
    {
        Uninitialise();
        PD3DSurface = other.PD3DSurface;
        AllocationSource = other.AllocationSource;
        if (AllocationSource == 2)
            PAllocatedMemory = other.PAllocatedMemory;
        if (PD3DSurface)
            PD3DSurface->AddRef();
        return *this;
    }
};
int main()
{
    IDirect3DSurface9 oldD3d; oldD3d.m_refs = 1;
    IDirect3DSurface9 newD3d; newD3d.m_refs = 1;
    int memory = 0;
    CSurface a; a.PD3DSurface = &oldD3d; a.AllocationSource = 1;
    CSurface b; b.PD3DSurface = &newD3d; b.AllocationSource = 2; b.PAllocatedMemory = &memory;
    CSurface& r = (a = b);
    if (&r != &a || a.PD3DSurface != &newD3d || a.AllocationSource != 2 || a.PAllocatedMemory != &memory
        || oldD3d.m_refs != 0 || newD3d.m_refs != 2 || g_releases != 1 || g_addRefs != 1) { std::printf("009f2e70_TEST FAIL\n"); return 1; }
    CSurface empty;
    a = empty;
    if (a.PD3DSurface != 0 || a.AllocationSource != 0 || newD3d.m_refs != 1 || g_releases != 2 || g_addRefs != 1) { std::printf("009f2e70_TEST FAIL\n"); return 1; }
    std::printf("009f2e70_TEST PASS\n");
    return 0;
}
