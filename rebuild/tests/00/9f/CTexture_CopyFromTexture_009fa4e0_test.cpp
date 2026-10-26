#include <cstdio>
#ifndef _MSC_VER
#define __stdcall
#endif
struct IDirect3DSurface9 { int level; int owner; };
class C2DBoxI { public: long left, top, right, bottom; };


struct IDirect3DTexture9 {                         // IUnknown + IDirect3DResource9 + IDirect3DBaseTexture9 + texture slots
    virtual long __stdcall QueryInterface(const void* iid, void** object);   // slot 0
    virtual unsigned long __stdcall AddRef();                                // slot 1
    virtual unsigned long __stdcall Release();                               // slot 2
    virtual long __stdcall GetDevice(void** device);                         // slot 3
    virtual long __stdcall SetPrivateData(const void* guid, const void* data, unsigned long size, unsigned long flags);
    virtual long __stdcall GetPrivateData(const void* guid, void* data, unsigned long* size);
    virtual long __stdcall FreePrivateData(const void* guid);
    virtual unsigned long __stdcall SetPriority(unsigned long priority);
    virtual unsigned long __stdcall GetPriority();
    virtual void __stdcall PreLoad();
    virtual long __stdcall GetType();
    virtual unsigned long __stdcall SetLOD(unsigned long lod);
    virtual unsigned long __stdcall GetLOD();
    virtual unsigned long __stdcall GetLevelCount();
    virtual long __stdcall SetAutoGenFilterType(long filter);
    virtual long __stdcall GetAutoGenFilterType();
    virtual void __stdcall GenerateMipSubLevels();                           // slot 16
    virtual long __stdcall GetLevelDesc(unsigned long level, void* desc);    // slot 17 (+0x44)
    virtual long __stdcall GetSurfaceLevel(unsigned long level, IDirect3DSurface9** surface);   // slot 18 (+0x48)
};

class CSurface {                                   // vtable 0x0122F84C
public:
    IDirect3DSurface9* PD3DSurface;                // +0x04
    unsigned long      AllocationSource;           // +0x08
    void*              PAllocatedMemory;           // +0x0c

    CSurface() : PD3DSurface(0), AllocationSource(0) { }
    CSurface(const CSurface& other);                                   // 0x009F2D60
    virtual ~CSurface() { Uninitialise(); }
    void Uninitialise();                                               // 0x009F2E20
    void InitialiseFromTextureMipmap(IDirect3DSurface9* mipmapSurface);   // 0x009F2F10
    void CopyFromSurface(const C2DBoxI* destinationBox, const CSurface& source, const C2DBoxI* sourceBox,
                         unsigned long filter);                        // 0x009F37E0
};

template <class T> inline const T& Min(const T& a, const T& b) { return a < b ? a : b; }

class CTexture {
public:
    IDirect3DTexture9* PD3DTexture;                // +0x00
    unsigned long      ByteLength : 28;            // +0x04
    unsigned long      AllocationSource : 4;       // +0x04
    CSurface GetSurfaceLevel(unsigned long level) const     // 0x009F9E00 (inlined here)
    {
        IDirect3DSurface9* d3dSurface;
        PD3DTexture->GetSurfaceLevel(level, &d3dSurface);
        CSurface surface;
        surface.InitialiseFromTextureMipmap(d3dSurface);
        return surface;
    }
    void CopyFromTexture(const CTexture& source, unsigned long filter);
};

void CTexture::CopyFromTexture(const CTexture& source, unsigned long filter)
{
    long sourceLevels = (long)source.PD3DTexture->GetLevelCount();
    long destinationLevels = (long)PD3DTexture->GetLevelCount();
    long levels = Min(sourceLevels, destinationLevels);
    for (long level = 0; level < levels; ++level) {
        CSurface sourceSurface = source.GetSurfaceLevel(level);
        CSurface destinationSurface = GetSurfaceLevel(level);
        destinationSurface.CopyFromSurface(0, sourceSurface, 0, filter);
    }
}
// ---- test doubles ----

static IDirect3DSurface9 g_surfaces[2][8];
struct CTestTexture : IDirect3DTexture9 {
    int id; unsigned long levels;
    virtual long __stdcall QueryInterface(const void*, void**) { return 0; }
    virtual unsigned long __stdcall AddRef() { return 1; }
    virtual unsigned long __stdcall Release() { return 0; }
    virtual long __stdcall GetDevice(void**) { return 0; }
    virtual long __stdcall SetPrivateData(const void*, const void*, unsigned long, unsigned long) { return 0; }
    virtual long __stdcall GetPrivateData(const void*, void*, unsigned long*) { return 0; }
    virtual long __stdcall FreePrivateData(const void*) { return 0; }
    virtual unsigned long __stdcall SetPriority(unsigned long) { return 0; }
    virtual unsigned long __stdcall GetPriority() { return 0; }
    virtual void __stdcall PreLoad() {}
    virtual long __stdcall GetType() { return 0; }
    virtual unsigned long __stdcall SetLOD(unsigned long) { return 0; }
    virtual unsigned long __stdcall GetLOD() { return 0; }
    virtual unsigned long __stdcall GetLevelCount() { return levels; }
    virtual long __stdcall SetAutoGenFilterType(long) { return 0; }
    virtual long __stdcall GetAutoGenFilterType() { return 0; }
    virtual void __stdcall GenerateMipSubLevels() {}
    virtual long __stdcall GetLevelDesc(unsigned long, void*) { return 0; }
    virtual long __stdcall GetSurfaceLevel(unsigned long level, IDirect3DSurface9** s) { *s = &g_surfaces[id][level]; return 0; }
};
static int g_live = 0, g_copies = 0, g_bad = 0; static unsigned long g_filter = 0;
CSurface::CSurface(const CSurface& o) : PD3DSurface(o.PD3DSurface), AllocationSource(o.AllocationSource) { ++g_live; }
void CSurface::Uninitialise() { --g_live; PD3DSurface = 0; }
void CSurface::InitialiseFromTextureMipmap(IDirect3DSurface9* s) { ++g_live; PD3DSurface = s; }
void CSurface::CopyFromSurface(const C2DBoxI* db, const CSurface& src, const C2DBoxI* sb, unsigned long f)
{
    ++g_copies; g_filter = f;
    if (db || sb || src.PD3DSurface->owner != 0 || PD3DSurface->owner != 1 || src.PD3DSurface->level != PD3DSurface->level) g_bad = 1;
}
long __stdcall IDirect3DTexture9::QueryInterface(const void*, void**) { return 0; }
unsigned long __stdcall IDirect3DTexture9::AddRef() { return 0; }
unsigned long __stdcall IDirect3DTexture9::Release() { return 0; }
long __stdcall IDirect3DTexture9::GetDevice(void**) { return 0; }
long __stdcall IDirect3DTexture9::SetPrivateData(const void*, const void*, unsigned long, unsigned long) { return 0; }
long __stdcall IDirect3DTexture9::GetPrivateData(const void*, void*, unsigned long*) { return 0; }
long __stdcall IDirect3DTexture9::FreePrivateData(const void*) { return 0; }
unsigned long __stdcall IDirect3DTexture9::SetPriority(unsigned long) { return 0; }
unsigned long __stdcall IDirect3DTexture9::GetPriority() { return 0; }
void __stdcall IDirect3DTexture9::PreLoad() {}
long __stdcall IDirect3DTexture9::GetType() { return 0; }
unsigned long __stdcall IDirect3DTexture9::SetLOD(unsigned long) { return 0; }
unsigned long __stdcall IDirect3DTexture9::GetLOD() { return 0; }
unsigned long __stdcall IDirect3DTexture9::GetLevelCount() { return 0; }
long __stdcall IDirect3DTexture9::SetAutoGenFilterType(long) { return 0; }
long __stdcall IDirect3DTexture9::GetAutoGenFilterType() { return 0; }
void __stdcall IDirect3DTexture9::GenerateMipSubLevels() {}
long __stdcall IDirect3DTexture9::GetLevelDesc(unsigned long, void*) { return 0; }
long __stdcall IDirect3DTexture9::GetSurfaceLevel(unsigned long, IDirect3DSurface9**) { return 0; }
int main()
{
    for (int t = 0; t < 2; ++t) for (int l = 0; l < 8; ++l) { g_surfaces[t][l].level = l; g_surfaces[t][l].owner = t; }
    CTestTexture srcTex, dstTex; srcTex.id = 0; srcTex.levels = 5; dstTex.id = 1; dstTex.levels = 3;
    CTexture src, dst; src.PD3DTexture = &srcTex; dst.PD3DTexture = &dstTex;
    dst.CopyFromTexture(src, 7);
    bool ok = g_copies == 3 && g_filter == 7 && !g_bad;
    dstTex.levels = 6; g_copies = 0;
    dst.CopyFromTexture(src, 2);
    ok = ok && g_copies == 5 && g_filter == 2 && !g_bad;
    srcTex.levels = 0; g_copies = 0;
    dst.CopyFromTexture(src, 1);
    ok = ok && g_copies == 0;
    if (!ok) { std::printf("009fa4e0_TEST FAIL\n"); return 1; }
    std::printf("009fa4e0_TEST PASS\n");
    return 0;
}