// Behaviour test for 0x00882FE0 CEnvironmentSpellEffectInterface::CScreenFilterParams::LerpTo(const CScreenFilterParams&, float)
#include <cstdio>
#include <cmath>

struct CRGBFloatColour {
    float R, G, B, A;
    CRGBFloatColour() { }
    CRGBFloatColour(float r, float g, float b, float a) : R(r), G(g), B(b), A(a) { }
    CRGBFloatColour operator*(float f) const { return CRGBFloatColour(R * f, G * f, B * f, A * f); }
    CRGBFloatColour operator+(const CRGBFloatColour& o) const { return CRGBFloatColour(R + o.R, G + o.G, B + o.B, A + o.A); }
};
struct CScreenFilterParams {
    float Alpha, Saturation, Contrast, Brightness;
    CRGBFloatColour Tint;
    CScreenFilterParams LerpTo(const CScreenFilterParams& to, float t)
    {
        if (t < 0.0f) t = 0.0f;
        else if (t > 1.0f) t = 1.0f;
        float s = 1.0f - t;
        CScreenFilterParams result;
        result.Alpha = Alpha * s + to.Alpha * t;
        result.Saturation = Saturation * s + to.Saturation * t;
        result.Contrast = Contrast * s + to.Contrast * t;
        result.Brightness = Brightness * s + to.Brightness * t;
        result.Tint = Tint * s + to.Tint * t;
        return result;
    }
};
static bool near(float a, float b) { return std::fabs(a - b) < 1e-5f; }
static bool check(const CScreenFilterParams& r, const CScreenFilterParams& a, const CScreenFilterParams& b, float u)
{
    float s = 1.0f - u;
    return near(r.Alpha, a.Alpha * s + b.Alpha * u) && near(r.Saturation, a.Saturation * s + b.Saturation * u)
        && near(r.Contrast, a.Contrast * s + b.Contrast * u) && near(r.Brightness, a.Brightness * s + b.Brightness * u)
        && near(r.Tint.R, a.Tint.R * s + b.Tint.R * u) && near(r.Tint.G, a.Tint.G * s + b.Tint.G * u)
        && near(r.Tint.B, a.Tint.B * s + b.Tint.B * u) && near(r.Tint.A, a.Tint.A * s + b.Tint.A * u);
}
int main()
{
    int fail = 0;
    CScreenFilterParams a, b;
    a.Alpha = 0.0f; a.Saturation = 1.0f; a.Contrast = 2.0f; a.Brightness = -1.0f; a.Tint = CRGBFloatColour(1.0f, 0.0f, 0.5f, 1.0f);
    b.Alpha = 1.0f; b.Saturation = 0.0f; b.Contrast = 4.0f; b.Brightness = 3.0f;  b.Tint = CRGBFloatColour(0.0f, 1.0f, 0.5f, 0.0f);
    if (!check(a.LerpTo(b, 0.25f), a, b, 0.25f)) fail |= 1;
    if (!check(a.LerpTo(b, -3.0f), a, b, 0.0f)) fail |= 2;      // clamped to 0: a
    if (!check(a.LerpTo(b, 7.0f), a, b, 1.0f)) fail |= 4;       // clamped to 1: b
    if (!near(a.LerpTo(b, 0.5f).Contrast, 3.0f)) fail |= 8;
    if (fail) { std::printf("00882fe0_TEST FAIL\n"); return 1; }
    std::printf("00882fe0_TEST PASS\n");
    return 0;
}