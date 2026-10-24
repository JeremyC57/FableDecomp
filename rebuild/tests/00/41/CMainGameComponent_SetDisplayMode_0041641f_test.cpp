#include <cstdio>
// Behaviour test for CMainGameComponent::SetDisplayMode (retail 0x0041641F): PreChangeResolution on the display
// engine, the requested mode handed to the display manager, then PostChangeResolution with the back surface's
// actual width, height and colour depth (which may differ from the request).
struct C2DExtentsI { long Width, Height; C2DExtentsI(long w, long h) : Width(w), Height(h) {} };
struct CPixelFormat { unsigned long m_depth; unsigned long GetColourDepth() const { return m_depth; } };
struct CSurface {
    long m_w, m_h; unsigned long m_depth;
    C2DExtentsI GetDimensions() const { return C2DExtentsI(m_w, m_h); }
    CPixelFormat GetPixelFormat() const { CPixelFormat f; f.m_depth = m_depth; return f; }
    unsigned long GetHeight() const { return (unsigned long)m_h; }
};
static int g_order = 0, g_preAt = 0, g_setAt = 0, g_postAt = 0;
static long g_reqW = 0, g_reqH = 0; static unsigned long g_reqDepth = 0, g_reqAA = 99, g_reqRefresh = 99;
static unsigned long g_postW = 0, g_postH = 0, g_postD = 0;
struct CDisplayManager {
    CSurface m_backSurface;
    int SetDisplayMode(const C2DExtentsI& r, unsigned long bpp, int aa, unsigned long refresh)
    {
        g_setAt = ++g_order; g_reqW = r.Width; g_reqH = r.Height; g_reqDepth = bpp; g_reqAA = aa; g_reqRefresh = refresh;
        m_backSurface.m_w = r.Width == 1000 ? 1024 : r.Width;   // the manager picks the nearest supported mode
        m_backSurface.m_h = r.Height == 700 ? 768 : r.Height;
        m_backSurface.m_depth = bpp == 24 ? 32 : bpp;
        return 0;
    }
};
static CDisplayManager g_manager;
static CDisplayManager* g_pDisplayManager = &g_manager;
struct CDisplayEngine { void PreChangeResolution() { g_preAt = ++g_order; } };
struct CMainGameComponent {
    CDisplayEngine* PDisplayEngine;
    void PostChangeResolution(unsigned long w, unsigned long h, unsigned long d) { g_postAt = ++g_order; g_postW = w; g_postH = h; g_postD = d; }
    void SetDisplayMode(unsigned long width, unsigned long height, unsigned char bitsPerPixel)
    {
        PDisplayEngine->PreChangeResolution();
        g_pDisplayManager->SetDisplayMode(C2DExtentsI(width, height), bitsPerPixel, 0, 0);
        CSurface* surface = &g_pDisplayManager->m_backSurface;
        unsigned long newWidth = surface->GetDimensions().Width;
        PostChangeResolution(newWidth, surface->GetHeight(), g_pDisplayManager->m_backSurface.GetPixelFormat().GetColourDepth());
    }
};
int main()
{
    CDisplayEngine engine; CMainGameComponent c; c.PDisplayEngine = &engine;
    c.SetDisplayMode(1000, 700, 24);
    int ok = g_preAt == 1 && g_setAt == 2 && g_postAt == 3 && g_reqW == 1000 && g_reqH == 700 && g_reqDepth == 24
          && g_reqAA == 0 && g_reqRefresh == 0 && g_postW == 1024 && g_postH == 768 && g_postD == 32;
    c.SetDisplayMode(640, 480, 16);
    ok = ok && g_postAt == 6 && g_postW == 640 && g_postH == 480 && g_postD == 16;
    if (!ok) { std::printf("0041641f_TEST FAIL\n"); return 1; }
    std::printf("0041641f_TEST PASS\n");
    return 0;
}