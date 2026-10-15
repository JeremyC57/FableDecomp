// Behaviour test for CSurface::InitialiseAsDepthBuffer (0x009F32A0): CreateDepthStencilSurface(w, h, fmt, multisample, 0, 0).
#include <cstdio>
struct Surf { int released; void Release() { ++released; } };
static Surf g_old, g_new;
static long g_hr; static unsigned int g_w, g_h, g_q; static long g_fmt, g_ms; static int g_discard;
struct Device {
    long CreateDepthStencilSurface(unsigned int w, unsigned int h, long fmt, long ms, unsigned int q, int discard, Surf** out, void**)
    { g_w = w; g_h = h; g_fmt = fmt; g_ms = ms; g_q = q; g_discard = discard; *out = &g_new; return g_hr; }
};
static Device g_dev;
static long g_table[2] = { 75, 80 };
struct Extents { long Width, Height; };
struct Format { long m_format; };
struct Surface {
    Surf* PD3DSurface; unsigned int AllocationSource;
    void Uninitialise() { if (PD3DSurface) { PD3DSurface->Release(); PD3DSurface = 0; AllocationSource = 0; } }
    bool InitialiseAsDepthBuffer(const Extents& size, const Format& format, long multisample, bool)
    {
        Uninitialise();
        long d3dFormat = g_table[format.m_format];
        long hr = g_dev.CreateDepthStencilSurface(size.Width, size.Height, d3dFormat, multisample, 0, 0, &PD3DSurface, 0);
        AllocationSource = 1;
        return hr >= 0 ? true : false;
    }
};
int main()
{
    Surface s; s.PD3DSurface = 0; s.AllocationSource = 0;
    Extents e = { 640, 480 }; Format f = { 1 }; g_hr = 1;
    if (!s.InitialiseAsDepthBuffer(e, f, 4, true) || g_old.released != 0 || g_w != 640 || g_h != 480 || g_fmt != 80 || g_ms != 4 || g_q != 0 || g_discard != 0 || s.AllocationSource != 1) { std::printf("009f32a0_TEST FAIL\n"); return 1; }
    s.PD3DSurface = &g_old; g_hr = -1;
    if (s.InitialiseAsDepthBuffer(e, f, 0, false) || g_old.released != 1) { std::printf("009f32a0_TEST FAIL\n"); return 1; }
    std::printf("009f32a0_TEST PASS\n");
    return 0;
}
