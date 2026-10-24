#include <cstdio>
#include <cmath>

// Model of CRGBFloatColour::Interpolate: this * (1 - t) + colour * t per channel.
struct CRGBFloatColour {
    float R, G, B, A;
    CRGBFloatColour() { }
    CRGBFloatColour(float r, float g, float b, float a) : R(r), G(g), B(b), A(a) { }
    CRGBFloatColour operator*(float f) const { return CRGBFloatColour(R * f, G * f, B * f, A * f); }
    CRGBFloatColour operator+(const CRGBFloatColour& c) const { return CRGBFloatColour(R + c.R, G + c.G, B + c.B, A + c.A); }
    CRGBFloatColour Interpolate(const CRGBFloatColour& c, float t) const { return *this * (1.0f - t) + c * t; }
};
static bool Near(float a, float b) { return std::fabs(a - b) < 1e-5f; }

int main()
{
    CRGBFloatColour a(0.0f, 1.0f, 0.5f, 1.0f), b(1.0f, 0.0f, 0.5f, 0.0f);
    CRGBFloatColour m = a.Interpolate(b, 0.25f);
    if (!Near(m.R, 0.25f) || !Near(m.G, 0.75f) || !Near(m.B, 0.5f) || !Near(m.A, 0.75f)) { std::printf("00b09b90_TEST FAIL\n"); return 1; }
    CRGBFloatColour e = a.Interpolate(b, 1.0f), s = a.Interpolate(b, 0.0f);
    if (!Near(e.R, 1.0f) || !Near(e.A, 0.0f) || !Near(s.G, 1.0f) || !Near(s.A, 1.0f)) { std::printf("00b09b90_TEST FAIL\n"); return 1; }
    std::printf("00b09b90_TEST PASS\n");
    return 0;
}