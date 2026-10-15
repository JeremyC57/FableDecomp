// Behaviour test for CSurface::InitialiseAsRenderTarget (0x009F3310): CreateRenderTarget(w, h, fmt, multisample, 0, lockable).
#include <cstdio>
struct Surf { int released; void Release() { ++released; } };
static Surf g_old, g_new;
static long g_hr; static unsigned int g_w, g_h; static long g_fmt, g_ms; static int g_lock;
struct Device {
    long CreateRenderTarget(unsigned int w, unsigned int h, long fmt, long ms, unsigned int, int lockable, Surf** out, void**)
    { g_w = w; g_h = h; g_fmt = fmt; g_ms = ms; g_lock = lockable; *out = &g_new; return g_hr; }
};
static Device g_dev;
static long g_table[2] = { 21, 22 };
struct Extents { long Width, Height; };
struct Format { long m_format; };
struct Surface {
    Surf* PD3DSurface; unsigned int AllocationSource;
    void Uninitialise() { if (PD3DSurface) { PD3DSurface->Release(); PD3DSurface = 0; AllocationSource = 0; } }
    bool InitialiseAsRenderTarget(const Extents& size, const Format& format, long multisample, bool lockable, bool)
    {
        Uninitialise();
        long width = size.Width, height = size.Height;
        long d3dFormat = g_table[format.m_format];
        long hr = g_dev.CreateRenderTarget(width, height, d3dFormat, multisample, 0, lockable, &PD3DSurface, 0);
        AllocationSource = 1;
        return hr >= 0 ? true : false;
    }
};
int main()
{
    Surface s; s.PD3DSurface = &g_old; s.AllocationSource = 7;
    Extents e = { 256, 128 }; Format f = { 0 }; g_hr = 0;
    if (!s.InitialiseAsRenderTarget(e, f, 2, true, false) || g_old.released != 1 || g_w != 256 || g_h != 128 || g_fmt != 21 || g_ms != 2 || g_lock != 1 || s.PD3DSurface != &g_new) { std::printf("009f3310_TEST FAIL\n"); return 1; }
    g_hr = -2;
    if (s.InitialiseAsRenderTarget(e, f, 0, false, false) || g_lock != 0 || s.AllocationSource != 1) { std::printf("009f3310_TEST FAIL\n"); return 1; }
    std::printf("009f3310_TEST PASS\n");
    return 0;
}