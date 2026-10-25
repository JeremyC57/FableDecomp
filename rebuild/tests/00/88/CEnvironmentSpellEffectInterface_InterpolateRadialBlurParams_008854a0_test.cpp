// Behaviour test for 0x008854A0 CEnvironmentSpellEffectInterface::InterpolateRadialBlurParams(float, const&, const&)
#include <cstdio>
#include <cmath>

struct CRGBFloatColour {
    float R, G, B, A;
    CRGBFloatColour() { }
    CRGBFloatColour(float r, float g, float b, float a) : R(r), G(g), B(b), A(a) { }
    CRGBFloatColour operator*(float f) const { return CRGBFloatColour(R * f, G * f, B * f, A * f); }
    CRGBFloatColour operator+(const CRGBFloatColour& o) const { return CRGBFloatColour(R + o.R, G + o.G, B + o.B, A + o.A); }
};
struct CRadialBlurParams {
    CRGBFloatColour ThresholdColour, IntensityColour;
    float Length, Fade, Range, Twist;
    CRadialBlurParams() { }
};
static CRadialBlurParams Interpolate(float t, const CRadialBlurParams& from, const CRadialBlurParams& to)
{
    if (t < 0.0f) t = 0.0f;
    else if (t > 1.0f) t = 1.0f;
    CRadialBlurParams result;
    result.Fade = from.Fade * (1.0f - t) + to.Fade * t;
    result.Length = from.Length * (1.0f - t) + to.Length * t;
    result.Range = from.Range * (1.0f - t) + to.Range * t;
    result.Twist = from.Twist * (1.0f - t) + to.Twist * t;
    result.ThresholdColour = from.ThresholdColour * (1.0f - t) + to.ThresholdColour * t;
    result.IntensityColour = from.IntensityColour * (1.0f - t) + to.IntensityColour * t;
    return result;
}
static bool near(float a, float b) { return std::fabs(a - b) < 1e-5f; }
int main()
{
    int fail = 0;
    CRadialBlurParams a, b;
    a.ThresholdColour = CRGBFloatColour(0.0f, 0.2f, 0.4f, 1.0f); a.IntensityColour = CRGBFloatColour(1.0f, 1.0f, 1.0f, 1.0f);
    a.Length = 10.0f; a.Fade = 0.0f; a.Range = 5.0f; a.Twist = -1.0f;
    b.ThresholdColour = CRGBFloatColour(1.0f, 0.2f, 0.0f, 0.0f); b.IntensityColour = CRGBFloatColour(0.0f, 0.5f, 2.0f, 3.0f);
    b.Length = 20.0f; b.Fade = 1.0f; b.Range = 1.0f; b.Twist = 1.0f;
    CRadialBlurParams r = Interpolate(0.25f, a, b);
    if (!near(r.Length, 12.5f) || !near(r.Fade, 0.25f) || !near(r.Range, 4.0f) || !near(r.Twist, -0.5f)) fail |= 1;
    if (!near(r.ThresholdColour.R, 0.25f) || !near(r.ThresholdColour.B, 0.3f) || !near(r.IntensityColour.A, 1.5f)) fail |= 2;
    r = Interpolate(-1.0f, a, b);
    if (!near(r.Length, 10.0f) || !near(r.IntensityColour.B, 1.0f)) fail |= 4;
    r = Interpolate(2.0f, a, b);
    if (!near(r.Length, 20.0f) || !near(r.IntensityColour.B, 2.0f) || !near(r.Twist, 1.0f)) fail |= 8;
    if (fail) { std::printf("008854a0_TEST FAIL\n"); return 1; }
    std::printf("008854a0_TEST PASS\n");
    return 0;
}