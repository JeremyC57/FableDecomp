// Behaviour test for CSurface::InitialiseAsImage (0x009F3230): old surface released, offscreen plain surface created in the
// scratch pool with the table's D3D format, AllocationSource = 1, result = hr >= 0.
#include <cstdio>
struct Surf { int released; void Release() { ++released; } };
static Surf g_old, g_new;
static long g_hr; static unsigned int g_w, g_h; static long g_fmt, g_pool;
struct Device {
    long CreateOffscreenPlainSurface(unsigned int w, unsigned int h, long fmt, long pool, Surf** out, void** shared)
    { g_w = w; g_h = h; g_fmt = fmt; g_pool = pool; *out = &g_new; return shared ? -1 : g_hr; }
};
static Device g_dev;
struct PixelInfo { long d3dFormat; long rest[8]; };
static PixelInfo g_table[3] = { {21}, {22}, {77} };
struct Extents { long Width, Height; };
struct Format { long m_format; };
struct Surface {
    Surf* PD3DSurface; unsigned int AllocationSource;
    void Uninitialise() { if (PD3DSurface) { PD3DSurface->Release(); PD3DSurface = 0; AllocationSource = 0; } }
    bool InitialiseAsImage(const Extents& size, const Format& format, bool)
    {
        Uninitialise();
        long width = size.Width, height = size.Height;
        Device* device = &g_dev;
        long d3dFormat = g_table[format.m_format].d3dFormat;
        long hr = device->CreateOffscreenPlainSurface(width, height, d3dFormat, 3, &PD3DSurface, 0);
        AllocationSource = 1;
        return hr >= 0 ? true : false;
    }
};
int main()
{
    Surface s; s.PD3DSurface = &g_old; s.AllocationSource = 2;
    Extents e = { 64, 32 }; Format f = { 2 };
    g_hr = 0;
    bool ok = s.InitialiseAsImage(e, f, false);
    if (!ok || g_old.released != 1 || s.PD3DSurface != &g_new || s.AllocationSource != 1 || g_w != 64 || g_h != 32 || g_fmt != 77 || g_pool != 3) { std::printf("009f3230_TEST FAIL\n"); return 1; }
    g_hr = -5;
    if (s.InitialiseAsImage(e, f, true)) { std::printf("009f3230_TEST FAIL\n"); return 1; }
    std::printf("009f3230_TEST PASS\n");
    return 0;
}