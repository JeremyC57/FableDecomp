// Behaviour test for CTexture::Initialise (0x009FA280): releases a held D3D texture (clearing the allocation
// source), derives the mip count when none is given (halve both extents while both are >= 8, plus one),
// creates the texture through the device with the pixel-format table's D3DFORMAT, and records
// AllocationSource = 1 and the byte length on success; a failed creation returns false.
#include <cstdio>
struct IDirect3DTexture9 {
    virtual long QueryInterface(const void*, void**) { return 0; }
    virtual unsigned long AddRef() { return 1; }
    virtual unsigned long Release() { ++releases; return 0; }
    int releases; IDirect3DTexture9() : releases(0) {}
};
struct SCreate { unsigned long w, h, levels, usage; long format, pool; void* shared; };
static SCreate g_create; static int g_creates = 0; static long g_createResult = 0; static IDirect3DTexture9 g_made;
struct IDirect3DDevice9 {
    virtual void v00() {} virtual void v01() {} virtual void v02() {} virtual void v03() {} virtual void v04() {} virtual void v05() {} virtual void v06() {} virtual void v07() {}
    virtual void v08() {} virtual void v09() {} virtual void v10() {} virtual void v11() {} virtual void v12() {} virtual void v13() {} virtual void v14() {} virtual void v15() {}
    virtual void v16() {} virtual void v17() {} virtual void v18() {} virtual void v19() {} virtual void v20() {} virtual void v21() {} virtual void v22() {}
    virtual long CreateTexture(unsigned long width, unsigned long height, unsigned long levels, unsigned long usage, long format, long pool, IDirect3DTexture9** ppTexture, void** pSharedHandle)
    { ++g_creates; SCreate c = { width, height, levels, usage, format, pool, (void*)pSharedHandle }; g_create = c; if (g_createResult < 0) return g_createResult; *ppTexture = &g_made; return 0; }
};
static IDirect3DDevice9 g_device;
struct CRenderSystem { unsigned char _pad00[0x58]; IDirect3DDevice9* m_pDevice; };
struct CSystemManager { unsigned char _pad00[0x60]; CRenderSystem* m_pRender; };
static CRenderSystem g_render; static CSystemManager g_system;
CSystemManager* GFGetSystemManager() { return &g_system; }
struct SPixelFormatInfo { long d3dFormat; long rest[8]; };
static SPixelFormatInfo g_pixelFormatInfo[4] = { { 21 }, { 22 }, { 23 }, { 24 } };
struct CPixelFormat { long m_format; long GetD3DFormat() const { return g_pixelFormatInfo[m_format].d3dFormat; } };
struct C2DExtentsI { long X, Y; };
static int g_calcs = 0;
struct CTexture {
    IDirect3DTexture9* PD3DTexture;
    unsigned long ByteLength : 28;
    unsigned long AllocationSource : 4;
    void CalcByteLength() { ++g_calcs; ByteLength = 0xABCDE; }
    bool Initialise(const C2DExtentsI& size, long mipLevels, const CPixelFormat& format, unsigned long usage, long pool, bool, bool);
};
bool CTexture::Initialise(const C2DExtentsI& size, long mipLevels, const CPixelFormat& format, unsigned long usage, long pool, bool, bool)
{
    if (PD3DTexture) {
        PD3DTexture->Release();
        PD3DTexture = 0;
        AllocationSource = 0;
    }
    long levels = mipLevels;
    if (levels <= 0) {
        unsigned long w = size.X;
        unsigned long h = size.Y;
        long n = 0;
        while (w >= 8 && h >= 8) {
            w >>= 1;
            h >>= 1;
            ++n;
        }
        levels = n + 1;
    }
    IDirect3DDevice9* device = GFGetSystemManager()->m_pRender->m_pDevice;
    if (device->CreateTexture(size.X, size.Y, levels, usage, format.GetD3DFormat(), pool, &PD3DTexture, 0) < 0)
        return false;
    AllocationSource = 1;
    CalcByteLength();
    return true;
}
static int fail(const char* why) { std::printf("009fa280_TEST FAIL: %s\n", why); return 1; }
int main()
{
    g_render.m_pDevice = &g_device; g_system.m_pRender = &g_render;
    CTexture t; t.PD3DTexture = 0; t.ByteLength = 0; t.AllocationSource = 0;
    C2DExtentsI size = { 64, 32 }; CPixelFormat fmt = { 2 };
    // 1. explicit mip count, no held texture
    if (!t.Initialise(size, 3, fmt, 0x10, 1, false, true)) return fail("creation reported failure");
    if (g_creates != 1 || g_create.w != 64 || g_create.h != 32 || g_create.levels != 3 || g_create.usage != 0x10 || g_create.format != 23 || g_create.pool != 1 || g_create.shared != 0) return fail("create arguments");
    if (t.PD3DTexture != &g_made || t.AllocationSource != 1 || t.ByteLength != 0xABCDE || g_calcs != 1) return fail("success state");
    // 2. derived mips (64x32: 64/32 -> 32/16 -> 16/8 -> 8/4 stops: 3 halvings + 1 = 4); the held texture is released first
    IDirect3DTexture9 old; t.PD3DTexture = &old;
    if (!t.Initialise(size, 0, fmt, 0, 0, false, false)) return fail("second creation");
    if (old.releases != 1 || g_create.levels != 4 || t.PD3DTexture != &g_made) return fail("release / derived mips");
    // 3. 7x7 gives one level; 8x8 gives two
    C2DExtentsI tiny = { 7, 7 }; t.PD3DTexture = 0; t.Initialise(tiny, -1, fmt, 0, 0, false, false); if (g_create.levels != 1) return fail("7x7 levels");
    C2DExtentsI eight = { 8, 8 }; t.Initialise(eight, 0, fmt, 0, 0, false, false); if (g_create.levels != 2) return fail("8x8 levels");
    // 4. failed creation: false, source cleared by the release path and not set
    IDirect3DTexture9 held; t.PD3DTexture = &held; t.AllocationSource = 1; g_createResult = -1; g_calcs = 0;
    if (t.Initialise(size, 1, fmt, 0, 0, false, false)) return fail("failure not reported");
    if (held.releases != 1 || t.PD3DTexture != 0 || t.AllocationSource != 0 || g_calcs != 0) return fail("failure state");
    std::printf("009fa280_TEST PASS\n");
    return 0;
}
