// Behaviour test for 0x00BE1C10 CEngineInternalPrimitive2DViewport::Update (C++03): the description's layer, viewport, FOVs
// (turns x 2pi), flags, alpha-mask bank / index / clip box are copied; the clip box is mapped to clip space with the render
// target's half extents, and the position factors map clip x in [left, right] and y in [top, bottom] onto [0, 1]
// (u = X*x + Z, v = Y*y + W).
#include <cmath>
#include <cstdio>

struct Box { float TLX, TLY, BRX, BRY; };
struct Vec4 { float X, Y, Z, W; Vec4() {} Vec4(float x, float y, float z, float w) : X(x), Y(y), Z(z), W(w) {} };
struct Desc { int Layer; Box Viewport; float HorizontalFOV, VerticalFOV; bool Use2DFOV; void* Bank; unsigned long Index;
              Box ClipBox; bool Mask, Filter; };
struct MaskInfo { void* Bank; unsigned long Index; Vec4 Factors; Box ClipBox; bool Mask, Filter; };
struct Viewport2D { Box Viewport; float HorizontalFOV, VerticalFOV; bool Use2DFOV; MaskInfo Info; int Layer; };

static long g_width = 640, g_height = 480;

static void Update(Viewport2D& self, const Desc& viewport)
{
    self.Layer = viewport.Layer;
    self.Viewport = viewport.Viewport;
    self.HorizontalFOV = viewport.HorizontalFOV * 6.2831855f;
    self.VerticalFOV = viewport.VerticalFOV * 6.2831855f;
    self.Use2DFOV = viewport.Use2DFOV;
    self.Info.Bank = viewport.Bank;
    self.Info.Index = viewport.Index;
    self.Info.ClipBox = viewport.ClipBox;
    float invHalfWidth = 1.0f / (g_width * 0.5f);
    float invHalfHeight = 1.0f / (g_height * 0.5f);
    float right = (invHalfWidth * viewport.ClipBox.BRX) - 1.0f;
    float bottom = 1.0f - (invHalfHeight * viewport.ClipBox.BRY);
    float left = (invHalfWidth * viewport.ClipBox.TLX) - 1.0f;
    float top = 1.0f - (invHalfHeight * viewport.ClipBox.TLY);
    self.Info.Factors = Vec4((1.0f / (right - left)), (1.0f / (bottom - top)), right / (left - right) + 1.0f, bottom / (top - bottom) + 1.0f);
    self.Info.Mask = viewport.Mask;
    self.Info.Filter = viewport.Filter;
}

static bool Near(float a, float b) { return std::fabs(a - b) < 1e-4f; }

static int Check(float tlx, float tly, float brx, float bry)
{
    Desc d; d.Layer = 67; d.Viewport.TLX = 1; d.Viewport.TLY = 2; d.Viewport.BRX = 3; d.Viewport.BRY = 4;
    d.HorizontalFOV = 0.25f; d.VerticalFOV = 0.125f; d.Use2DFOV = true; d.Bank = &d; d.Index = 7;
    d.ClipBox.TLX = tlx; d.ClipBox.TLY = tly; d.ClipBox.BRX = brx; d.ClipBox.BRY = bry; d.Mask = true; d.Filter = false;
    Viewport2D v; Update(v, d);
    float xl = tlx / (g_width * 0.5f) - 1.0f, xr = brx / (g_width * 0.5f) - 1.0f;
    float yt = 1.0f - tly / (g_height * 0.5f), yb = 1.0f - bry / (g_height * 0.5f);
    const Vec4& f = v.Info.Factors;
    bool ok = v.Layer == 67 && v.Viewport.BRY == 4 && Near(v.HorizontalFOV, 1.5707964f) && Near(v.VerticalFOV, 0.7853982f) &&
              v.Use2DFOV && v.Info.Bank == &d && v.Info.Index == 7 && v.Info.ClipBox.BRX == brx && v.Info.Mask && !v.Info.Filter &&
              Near(f.X * xl + f.Z, 0.0f) && Near(f.X * xr + f.Z, 1.0f) && Near(f.Y * yt + f.W, 0.0f) && Near(f.Y * yb + f.W, 1.0f);
    if (!ok) { std::printf("FAIL box %g %g %g %g\n", tlx, tly, brx, bry); return 1; }
    return 0;
}

int main()
{
    int fails = 0;
    fails += Check(0, 0, 640, 480);
    fails += Check(100, 50, 300, 250);
    g_width = 1024; g_height = 768;
    fails += Check(12, 700, 1000, 760);
    if (fails) return 1;
    std::printf("00be1c10_TEST PASS\n");
    return 0;
}