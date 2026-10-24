#include <cstdio>
#include <math.h>

// Model of CEngineSurfaceCompositionManager::CalcColourDifference (retail 0x00B2AF50).
struct CRGBColour { unsigned char B, G, R, A; };

static float CalcColourDifference(const CRGBColour& a, const CRGBColour& b)
{
    float r = (float)(a.R - b.R);
    float g = (float)(a.G - b.G);
    float bl = (float)(a.B - b.B);
    float al = (float)(a.A - b.A);
    return (float)sqrt(r * r + g * g + bl * bl + al * al);
}

static bool Near(float a, float b) { float d = a - b; return d > -1e-3f && d < 1e-3f; }

int main()
{
    CRGBColour a, b;
    a.B = 10; a.G = 20; a.R = 30; a.A = 255;
    b = a;
    if (!Near(CalcColourDifference(a, b), 0.0f)) { std::printf("00b2af50_TEST FAIL\n"); return 1; }
    b.R = 33; b.G = 24;                         // dr = -3, dg = -4 -> 5
    if (!Near(CalcColourDifference(a, b), 5.0f)) { std::printf("00b2af50_TEST FAIL\n"); return 1; }
    b.B = 10; b.A = 255; a.R = 0; a.G = 0; a.B = 0; a.A = 0; b.R = 0; b.G = 0; b.B = 0; b.A = 2;   // da = -2
    if (!Near(CalcColourDifference(a, b), 2.0f)) { std::printf("00b2af50_TEST FAIL\n"); return 1; }
    std::printf("00b2af50_TEST PASS\n");
    return 0;
}