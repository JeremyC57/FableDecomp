#include <cstdio>
#ifndef _MSC_VER
#define __fastcall
#endif
// Test model: a 1280x960 render target; the GUI layout space is 640x480.
struct C2DVector { float X, Y; C2DVector() {} C2DVector(float x, float y) : X(x), Y(y) {} };
struct C2DBoxF {
    float TLX, TLY, BRX, BRY;
    C2DBoxF() {}
    C2DBoxF(float tlx, float tly, float brx, float bry) : TLX(tlx), TLY(tly), BRX(brx), BRY(bry) {}
};
struct C2DExtentsI { long Width, Height; };
static int g_dimsCalls = 0;
struct CDisplayManager {
    long W, H;
    C2DExtentsI GetRenderTargetDimensions() { ++g_dimsCalls; C2DExtentsI e; e.Width = W; e.Height = H; return e; }
};
static CDisplayManager g_display = { 1280, 960 };
static CDisplayManager* g_pDisplayManager = &g_display;
static bool Near(float a, float b) { float d = a - b; return d < 0.01f && d > -0.01f; }

C2DVector __fastcall GFAnchorToBottomLeft(const C2DVector& pos)
{
    return C2DVector(pos.X, (float)g_pDisplayManager->GetRenderTargetDimensions().Height + pos.Y - 480.0f);
}
int main()
{
    C2DVector p(100.0f, 50.0f);
    C2DVector r = GFAnchorToBottomLeft(p);
    if (!Near(r.X, 100.0f) || !Near(r.Y, 530.0f) || g_dimsCalls != 1) { std::printf("00643cda_TEST FAIL\n"); return 1; }
    g_display.H = 480;
    r = GFAnchorToBottomLeft(p);
    if (!Near(r.X, 100.0f) || !Near(r.Y, 50.0f)) { std::printf("00643cda_TEST FAIL\n"); return 1; }
    std::printf("00643cda_TEST PASS\n");
    return 0;
}