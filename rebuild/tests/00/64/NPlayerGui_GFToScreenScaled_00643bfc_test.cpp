#include <cstdio>
#ifndef _MSC_VER
#define __fastcall
#endif
// Test model: a 1280x960 render target; the GUI layout space is 640x480.
class C2DVector { public: float X, Y; C2DVector() {} C2DVector(float x, float y) : X(x), Y(y) {} };
class C2DBoxF { public:
    float TLX, TLY, BRX, BRY;
    C2DBoxF() {}
    C2DBoxF(float tlx, float tly, float brx, float bry) : TLX(tlx), TLY(tly), BRX(brx), BRY(bry) {}
};
class C2DExtentsI { public: long Width, Height; };
static int g_dimsCalls = 0;
class CDisplayManager { public:
    long W, H;
    C2DExtentsI GetRenderTargetDimensions() { ++g_dimsCalls; C2DExtentsI e; e.Width = W; e.Height = H; return e; }
};
static CDisplayManager g_display = { 1280, 960 };
static CDisplayManager* g_pDisplayManager = &g_display;
static bool Near(float a, float b) { float d = a - b; return d < 0.01f && d > -0.01f; }

C2DBoxF __fastcall GFToScreenScaled(const C2DBoxF& box)
{
    return C2DBoxF((float)g_pDisplayManager->GetRenderTargetDimensions().Width * box.TLX * (1.0f / 640.0f),
                   (float)g_pDisplayManager->GetRenderTargetDimensions().Height * box.TLY * (1.0f / 480.0f),
                   (float)g_pDisplayManager->GetRenderTargetDimensions().Width * box.BRX * (1.0f / 640.0f),
                   (float)g_pDisplayManager->GetRenderTargetDimensions().Height * box.BRY * (1.0f / 480.0f));
}
int main()
{
    C2DBoxF b(64.0f, 48.0f, 320.0f, 240.0f);
    C2DBoxF r = GFToScreenScaled(b);
    if (!Near(r.TLX, 128.0f) || !Near(r.TLY, 96.0f) || !Near(r.BRX, 640.0f) || !Near(r.BRY, 480.0f)) { std::printf("00643bfc_TEST FAIL\n"); return 1; }
    if (g_dimsCalls != 4) { std::printf("00643bfc_TEST FAIL\n"); return 1; }
    std::printf("00643bfc_TEST PASS\n");
    return 0;
}