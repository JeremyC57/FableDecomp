// Behaviour test for 0x00643CDA NPlayerGui::GFAnchorToBottomLeft (links the REAL row object).
// The display manager's GetRenderTargetDimensions is provided here (counts calls, checks the receiver); a 640x480 GUI position is moved by
// (render-target height - 480) in Y, X unchanged.
#include <stdio.h>
class C2DVector { public: float X, Y; C2DVector() {} C2DVector(float x, float y) : X(x), Y(y) {} };   // user constructors: returned through the hidden pointer, as the real class
class C2DExtentsI { public: long X, Y; C2DExtentsI() {} C2DExtentsI(long x, long y) : X(x), Y(y) {} };
class CDisplayManager { public: const C2DExtentsI GetRenderTargetDimensions(); };
static long g_w, g_h; static int g_calls, g_fail; static unsigned char g_obj[8];
CDisplayManager* GPDisplayManager = (CDisplayManager*)g_obj;
const C2DExtentsI CDisplayManager::GetRenderTargetDimensions() { ++g_calls; if ((void*)this != (void*)g_obj) ++g_fail; return C2DExtentsI(g_w, g_h); }
namespace NPlayerGui { C2DVector __fastcall GFAnchorToBottomLeft(const C2DVector& pos); }
static void Case(float x, float y, long w, long h)
{
    g_w = w; g_h = h; g_calls = 0;
    C2DVector p; p.X = x; p.Y = y;
    C2DVector r = NPlayerGui::GFAnchorToBottomLeft(p);
    float want = (float)h + y - 480.0f;
    if (r.X != x || r.Y != want || g_calls != 1 || p.X != x || p.Y != y) { ++g_fail; printf("FAIL %g %g %ld %ld -> %g %g (want %g %g) calls %d\n", x, y, w, h, r.X, r.Y, x, want, g_calls); }
}
int main()
{
    Case(100.0f, 50.0f, 1280, 960);     // taller target: Y moves down by 480
    Case(100.0f, 50.0f, 640, 480);      // the layout size: unchanged
    Case(-7.5f, 479.0f, 800, 600);
    Case(3.0f, -20.0f, 1024, 768);
    Case(640.0f, 0.0f, 480, 640);       // width and height differ both ways: only the height is used
    Case(0.25f, 12.5f, 2048, 0);
    printf(g_fail ? "00643cda_TEST FAIL\n" : "00643cda_TEST PASS\n");
    return g_fail ? 1 : 0;
}