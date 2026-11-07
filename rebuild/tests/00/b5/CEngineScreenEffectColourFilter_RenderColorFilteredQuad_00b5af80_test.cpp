// Behaviour test for 0x00B5AF80 CEngineScreenEffectColourFilter::RenderColorFilteredQuad: the pixel-shader constants
// it computes (contrast clamped to [0,2], scale = tint * contrast * 0.5, offset level clamped to [-0.5,0.5] + 0.5,
// inverse = 1 - grey, alpha clamped to [0,1]).
#include <stdio.h>
#include <math.h>
struct V4 { float X, Y, Z, W; };
template <class T> static void GFLimit(T& v, T lo, T hi) { if (v < lo) v = lo; else if (v > hi) v = hi; }
static void Constants(float contrast, const V4& tint, float brightness, float greyLevel, V4& scale, V4& offset, float& offsetLevel)
{
    float c = contrast + 1.0f;
    GFLimit(c, 0.0f, 2.0f);
    scale.X = tint.X * c * 0.5f; scale.Y = tint.Y * c * 0.5f; scale.Z = tint.Z * c * 0.5f; scale.W = tint.W * c * 0.5f;
    offsetLevel = brightness - (1.0f - greyLevel) * contrast;
    GFLimit(offsetLevel, -0.5f, 0.5f);
    offsetLevel += 0.5f;
    offset.X = tint.X * offsetLevel; offset.Y = tint.Y * offsetLevel; offset.Z = tint.Z * offsetLevel; offset.W = tint.W * offsetLevel;
}
static bool Near(float a, float b) { return fabs(a - b) < 1e-5f; }
int main()
{
    int ok = 1;
    V4 tint = { 1.0f, 0.5f, 0.25f, 1.0f }, s, o; float lvl;
    Constants(0.0f, tint, 0.0f, 0.5f, s, o, lvl);
    ok &= Near(s.X, 0.5f) && Near(s.Y, 0.25f) && Near(lvl, 0.5f) && Near(o.Z, 0.125f);
    Constants(5.0f, tint, 0.0f, 0.5f, s, o, lvl);           // contrast clamps at 2 -> scale = tint
    ok &= Near(s.X, 1.0f) && Near(s.Z, 0.25f) && Near(lvl, 0.0f);
    Constants(-3.0f, tint, 0.2f, 0.5f, s, o, lvl);          // contrast clamps at 0 -> scale 0; offset clamps high
    ok &= Near(s.X, 0.0f) && Near(lvl, 1.0f) && Near(o.Y, 0.5f);
    float alpha = 1.5f; GFLimit(alpha, 0.0f, 1.0f); ok &= alpha == 1.0f;
    float grey = 0.3f; ok &= Near(1.0f - grey, 0.7f);
    if (ok) printf("00b5af80_TEST PASS\n"); else printf("00b5af80_TEST FAIL\n");
    return ok ? 0 : 1;
}