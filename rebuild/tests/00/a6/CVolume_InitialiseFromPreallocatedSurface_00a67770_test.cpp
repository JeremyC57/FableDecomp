#include <cstdio>
static int g_addRefs = 0, g_releases = 0;
struct IDirect3DVolume9 {
    long m_refs;
    virtual unsigned long AddRef() { ++g_addRefs; return ++m_refs; }
    virtual unsigned long Release() { ++g_releases; return --m_refs; }
    virtual ~IDirect3DVolume9() {}
};
static unsigned long g_VolumeMemoryBytes = 0, g_GraphicsMemoryBytes = 0;
static unsigned long g_byteLength = 0;
class CVolume { public:
    void* __vftable;
    IDirect3DVolume9* PD3DVolume;
    void ReleaseVolume() { if (PD3DVolume) { PD3DVolume->Release(); PD3DVolume = 0; } }
    unsigned long GetByteLength() const { return g_byteLength; }
    unsigned long Sizeof() const
    {
        unsigned long size = 8;
        if (PD3DVolume)
            size += GetByteLength() + 4;
        return size;
    }
    void InitialiseFromPreallocatedSurface(IDirect3DVolume9* volume)
    {
        ReleaseVolume();
        PD3DVolume = volume;
        PD3DVolume->AddRef();
        g_VolumeMemoryBytes += Sizeof();
        g_GraphicsMemoryBytes += Sizeof();
    }
};
int main()
{
    IDirect3DVolume9 oldV; oldV.m_refs = 1;
    IDirect3DVolume9 newV; newV.m_refs = 1;
    CVolume v; v.__vftable = 0; v.PD3DVolume = &oldV;
    g_byteLength = 1000;
    v.InitialiseFromPreallocatedSurface(&newV);
    if (v.PD3DVolume != &newV || oldV.m_refs != 0 || newV.m_refs != 2 || g_VolumeMemoryBytes != 1012 || g_GraphicsMemoryBytes != 1012) { std::printf("00a67770_TEST FAIL\n"); return 1; }
    CVolume w; w.__vftable = 0; w.PD3DVolume = 0;
    w.InitialiseFromPreallocatedSurface(&oldV);
    if (w.PD3DVolume != &oldV || g_releases != 1 || g_addRefs != 2 || g_VolumeMemoryBytes != 2024) { std::printf("00a67770_TEST FAIL\n"); return 1; }
    std::printf("00a67770_TEST PASS\n");
    return 0;
}