// Behaviour test for NFractal::CPerlinNoise::Get2DNoise (0x00F36010): the row's logic on stand-alone tables, floored with the shared
// GFFloatToLongFloor (the retail asm under VC7.1, its FABLE_PORTABLE twin elsewhere), checked against a double-precision reference.
#ifndef _MSC_VER
#define FABLE_PORTABLE
#endif
#include <stdio.h>
#include <math.h>
#include "engine/GFFloatToLongFloor.h"
struct V2 { float X, Y; };
static unsigned long IndexTable[256];
static float GX[256], GY[256];
static float SCurve(float t) { return (t * t * (3.0f - 2.0f * t)); }
static float Interp(float a, float b, float t) { return (a + (b - a) * t); }
static unsigned long LookupIndex(unsigned long i) { return IndexTable[i & 0xff]; }
static float Get2DNoise(const V2& point)
{
    long x_int = GFFloatToLongFloor(point.X);
    long y_int = GFFloatToLongFloor(point.Y);
    float v0_x = point.X - x_int, v0_y = point.Y - y_int;
    float v1_x = v0_x - 1.0f, v1_y = v0_y - 1.0f;
    float tx = SCurve(v0_x), ty = SCurve(v0_y);
    unsigned long b00 = LookupIndex(LookupIndex(x_int) + y_int);
    unsigned long b10 = LookupIndex(LookupIndex(x_int + 1) + y_int);
    unsigned long b01 = LookupIndex(LookupIndex(x_int) + y_int + 1);
    unsigned long b11 = LookupIndex(LookupIndex(x_int + 1) + y_int + 1);
    float value00 = GX[b00 & 0xff] * v0_x + GY[b00 & 0xff] * v0_y;
    float value10 = GX[b10 & 0xff] * v1_x + GY[b10 & 0xff] * v0_y;
    float value01 = GX[b01 & 0xff] * v0_x + GY[b01 & 0xff] * v1_y;
    float value11 = GX[b11 & 0xff] * v1_x + GY[b11 & 0xff] * v1_y;
    return Interp(Interp(value00, value10, tx), Interp(value01, value11, tx), ty);
}
static double Ref(double x, double y)
{
    long xi = (long)floor(x), yi = (long)floor(y);
    double fx = x - xi, fy = y - yi, sx = fx * fx * (3 - 2 * fx), sy = fy * fy * (3 - 2 * fy);
    unsigned long c[2][2];
    for (int i = 0; i < 2; ++i) for (int j = 0; j < 2; ++j) c[i][j] = IndexTable[(IndexTable[(xi + i) & 255] + yi + j) & 255] & 255;
    double v[2][2];
    for (int i = 0; i < 2; ++i) for (int j = 0; j < 2; ++j) v[i][j] = GX[c[i][j]] * (fx - i) + GY[c[i][j]] * (fy - j);
    double a = v[0][0] + (v[1][0] - v[0][0]) * sx, b = v[0][1] + (v[1][1] - v[0][1]) * sx;
    return a + (b - a) * sy;
}
int main()
{
    for (int i = 0; i < 256; ++i) { IndexTable[i] = (unsigned long)((i * 89 + 7) & 255); GX[i] = (float)((i * 37) % 101) / 50.0f - 1.0f; GY[i] = (float)((i * 53) % 97) / 48.0f - 1.0f; }
    static const float pts[][2] = { {0.25f, 0.75f}, {3.0f, 5.0f}, {-2.5f, 7.125f}, {-0.0f, -1.0f}, {12.9f, -3.3f}, {255.5f, 256.25f}, {1.0f, 0.5f}, {-7.75f, -0.125f} };
    int fails = 0;
    for (unsigned k = 0; k < sizeof(pts) / sizeof(pts[0]); ++k) {
        V2 p = { pts[k][0], pts[k][1] };
        float got = Get2DNoise(p); double want = Ref(p.X, p.Y);
        if (fabs(got - want) > 1e-5) { ++fails; printf("pt %u: %f v %f\n", k, got, want); }
    }
    V2 q = { 3.0f, 5.0f };                                   // a lattice point: every offset is 0 or -1, the blend weight 0 -> 0
    if (Get2DNoise(q) != 0.0f) ++fails;
    printf(fails ? "00f36010_TEST FAIL\n" : "00f36010_TEST PASS\n");
    return fails ? 1 : 0;
}
