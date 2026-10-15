#include <cstdio>
// Behaviour test for CRenderTarget::~CRenderTarget (0x004979A0): depth then colour surface uninitialised, base last.
static int g_order[4]; static int g_n = 0;
struct CSurface {
    int id;
    void Uninitialise() { g_order[g_n++] = id; }
    virtual ~CSurface() { Uninitialise(); }
};
struct CBase { virtual void v00() { } ~CBase() { g_order[g_n++] = 0; } };
struct CRenderTarget : CBase {
    CSurface ColourSurface; CSurface DepthSurface;
    CRenderTarget() { ColourSurface.id = 1; DepthSurface.id = 2; }
    ~CRenderTarget() { }
};
int main()
{
    { CRenderTarget t; }
    int fails = 0;
    if (g_n != 3 || g_order[0] != 2 || g_order[1] != 1 || g_order[2] != 0) ++fails;
    if (fails) { std::printf("004979a0_TEST FAIL\n"); return 1; }
    std::printf("004979a0_TEST PASS\n");
    return 0;
}