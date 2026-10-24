#include <cstdio>
#include <cmath>
#ifndef _MSC_VER
#define __fastcall
#endif
// Behaviour test for CEngineWaterRenderer::SetRenderTargetToSurface(CRenderTarget&, const CSurface&) @ 0x00B7AD20: attach
// the surface, make the target current, then set the viewport (0, 0, GFAXToVX(width), GFAYToVY(height)).
static char g_log[8];
static int g_n = 0;
static void Log(char c) { if (g_n < 7) g_log[g_n++] = c; g_log[g_n] = 0; }
class C2DExtentsI {
public:
    long X;
    long Y;
    C2DExtentsI() { }
};
class CSurface {
public:
    long W, H;
    C2DExtentsI GetDimensions() const { Log('D'); C2DExtentsI e; e.X = W; e.Y = H; return e; }
    unsigned long GetWidth() const { return GetDimensions().X; }
    unsigned long GetHeight() const { return GetDimensions().Y; }
};
class CRenderTarget {
public:
    const CSurface* Attached;
    void AttachTarget(const CSurface& surface) { Log('A'); Attached = &surface; }
};
class CDisplayManager {
public:
    CRenderTarget* Current;
    bool SetRenderTarget(CRenderTarget& target) { Log('T'); Current = &target; return true; }
};
class C2DBoxF {
public:
    float X0, Y0, X1, Y1;
    C2DBoxF(float x0, float y0, float x1, float y1) : X0(x0), Y0(y0), X1(x1), Y1(y1) { }
};
class CRenderManager {
public:
    float Box[4];
    void SetViewport(const C2DBoxF& box) { Log('V'); Box[0] = box.X0; Box[1] = box.Y0; Box[2] = box.X1; Box[3] = box.Y1; }
};
static float __fastcall GFAXToVX(float x) { return x / 640.0f; }
static float __fastcall GFAYToVY(float y) { return y / 480.0f; }
static CRenderManager g_renderManager;
static CDisplayManager g_displayManager;
static CRenderManager* g_pRenderManager = &g_renderManager;
static CDisplayManager* g_pDisplayManager = &g_displayManager;
class CEngineWaterRenderer {
public:
    void SetRenderTargetToSurface(CRenderTarget& target, const CSurface& surface)
    {
        target.AttachTarget(surface);
        g_pDisplayManager->SetRenderTarget(target);
        g_pRenderManager->SetViewport(C2DBoxF(0.0f, 0.0f, GFAXToVX((float)surface.GetWidth()), GFAYToVY((float)surface.GetHeight())));
    }
};
static bool Near(float a, float b) { return std::fabs(a - b) <= 1e-5f * (1.0f + std::fabs(b)); }
int main()
{
    CEngineWaterRenderer w;
    CRenderTarget target;
    CSurface surface;
    surface.W = 256; surface.H = 120;
    w.SetRenderTargetToSurface(target, surface);
    if (target.Attached != &surface || g_displayManager.Current != &target) { std::printf("00b7ad20_TEST FAIL\n"); return 1; }
    if (g_log[0] != 'A' || g_log[1] != 'T' || g_log[g_n - 1] != 'V' || g_n != 5) { std::printf("00b7ad20_TEST FAIL\n"); return 1; }
    if (!Near(g_renderManager.Box[0], 0.0f) || !Near(g_renderManager.Box[1], 0.0f) || !Near(g_renderManager.Box[2], 0.4f) || !Near(g_renderManager.Box[3], 0.25f)) { std::printf("00b7ad20_TEST FAIL\n"); return 1; }
    std::printf("00b7ad20_TEST PASS\n");
    return 0;
}