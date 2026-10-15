#include <cstdio>

static int g_uninit = 0, g_assign = 0;

struct CSurface {
    void* m_pSurface; void* m_pTexture; long m_format;
    CSurface() : m_pSurface(0), m_pTexture(0), m_format(0) { }
    bool IsInitialised() const { return m_pSurface != 0; }
    void Uninitialise() { ++g_uninit; m_pSurface = 0; }
    CSurface& operator=(const CSurface& o) { ++g_assign; m_pSurface = o.m_pSurface; m_pTexture = o.m_pTexture; m_format = o.m_format; return *this; }
};

struct CRenderTarget {
    CSurface m_colour;
    CSurface m_depth;
    void AttachTarget(const CSurface& target)
    {
        if (m_colour.IsInitialised())
            m_colour.Uninitialise();
        m_colour = target;
    }
};

int main()
{
    CRenderTarget t; int a = 1, b = 2; CSurface s; s.m_pSurface = &a; s.m_format = 21;
    t.AttachTarget(s);
    if (g_uninit != 0 || g_assign != 1 || t.m_colour.m_pSurface != &a || t.m_colour.m_format != 21 || t.m_depth.m_pSurface)
    { std::printf("00a10e60_TEST FAIL\n"); return 1; }
    s.m_pSurface = &b; t.AttachTarget(s);
    if (g_uninit != 1 || g_assign != 2 || t.m_colour.m_pSurface != &b) { std::printf("00a10e60_TEST FAIL\n"); return 1; }
    std::printf("00a10e60_TEST PASS\n");
    return 0;
}