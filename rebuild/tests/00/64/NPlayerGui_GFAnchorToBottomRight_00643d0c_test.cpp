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
    const C2DExtentsI GetRenderTargetDimensions() { ++g_dimsCalls; C2DExtentsI e; e.Width = W; e.Height = H; return e; }
};
static CDisplayManager g_display = { 1280, 960 };
static CDisplayManager* g_pDisplayManager = &g_display;
static bool Near(float a, float b) { float d = a - b; return d < 0.01f && d > -0.01f; }

C2DVector __fastcall GFAnchorToBottomRight(const C2DVector& pos)
{
    return C2DVector((float)g_pDisplayManager->GetRenderTargetDimensions().Width + pos.X - 640.0f,
                     (float)g_pDisplayManager->GetRenderTargetDimensions().Height + pos.Y - 480.0f);
}
int main()
{
    C2DVector p(100.0f, 50.0f);
    C2DVector r = GFAnchorToBottomRight(p);
    if (!Near(r.X, 740.0f) || !Near(r.Y, 530.0f) || g_dimsCalls != 2) { std::printf("00643d0c_TEST FAIL\n"); return 1; }
    std::printf("00643d0c_TEST PASS\n");
    return 0;
}