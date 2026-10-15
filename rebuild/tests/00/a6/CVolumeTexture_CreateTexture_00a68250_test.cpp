#include <cstdio>
#include <cstring>
struct IDirect3DDevice9 { int id; };
struct CRenderSystem { IDirect3DDevice9* m_pDevice; };
struct CSystemManager { CRenderSystem* m_pRender; };
static IDirect3DDevice9 g_device; static CRenderSystem g_render; static CSystemManager g_system;
CSystemManager* GFGetSystemManager() { g_render.m_pDevice = &g_device; g_system.m_pRender = &g_render; return &g_system; }
struct IDirect3DVolumeTexture9 { int id; };
static IDirect3DVolumeTexture9 g_created; static long g_args[8]; static long g_result;
long D3DXCreateVolumeTexture(IDirect3DDevice9* device, unsigned long w, unsigned long h, unsigned long d, unsigned long mips,
    unsigned long usage, long format, long pool, IDirect3DVolumeTexture9** out)
{
    g_args[0] = device == &g_device; g_args[1] = w; g_args[2] = h; g_args[3] = d; g_args[4] = mips; g_args[5] = usage; g_args[6] = format; g_args[7] = pool;
    if (g_result >= 0) *out = &g_created;
    return g_result;
}
struct C3DVectorI { long X, Y, Z; };
struct SPixelFormatInfo { long d3dFormat; long rest[8]; };
static SPixelFormatInfo g_pixelFormatInfo[3] = { { 21, {0} }, { 22, {0} }, { 50, {0} } };
struct CPixelFormat { long m_format; };
struct CVolumeTexture {
    IDirect3DVolumeTexture9* m_pTexture; long m_format; long m_levels;
    bool CreateTexture(const C3DVectorI& size, unsigned long mipLevels, unsigned long usage, const CPixelFormat& format, long pool, long)
    {
        IDirect3DDevice9* device = GFGetSystemManager()->m_pRender->m_pDevice;
        long d3dFormat = g_pixelFormatInfo[format.m_format].d3dFormat;
        return D3DXCreateVolumeTexture(device, size.X, size.Y, size.Z, mipLevels, usage, d3dFormat, pool, &m_pTexture) >= 0 ? true : false;
    }
};
int main()
{
    int ok = 1;
    CVolumeTexture v; v.m_pTexture = 0; v.m_format = 0; v.m_levels = 0;
    C3DVectorI size; size.X = 8; size.Y = 16; size.Z = 32; CPixelFormat f; f.m_format = 2;
    bool r = v.CreateTexture(size, 1, 0x200, f, 1, 99);
    if (!r || v.m_pTexture != &g_created || !g_args[0] || g_args[1] != 8 || g_args[2] != 16 || g_args[3] != 32) ok = 0;
    if (g_args[4] != 1 || g_args[5] != 0x200 || g_args[6] != 50 || g_args[7] != 1) ok = 0;
    g_result = -5;
    if (v.CreateTexture(size, 1, 0, f, 0, 0)) ok = 0;
    if (!ok) { std::printf("00a68250_TEST FAIL\n"); return 1; }
    std::printf("00a68250_TEST PASS\n");
    return 0;
}