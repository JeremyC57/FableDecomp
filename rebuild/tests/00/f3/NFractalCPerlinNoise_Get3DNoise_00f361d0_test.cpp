// Behaviour test for NFractal::CPerlinNoise::Get3DNoise (0x00F361D0): the row's logic on stand-alone tables, floored with the shared
// GFFloatToLongFloor (the retail asm under VC7.1, its FABLE_PORTABLE twin elsewhere), checked against a double-precision reference.
#ifndef _MSC_VER
#define FABLE_PORTABLE
#endif
#include <stdio.h>
#include <math.h>
#include "engine/GFFloatToLongFloor.h"
struct V3 { float X, Y, Z; };
static unsigned long IndexTable[256];
static V3 Grad[256];
static float SCurve(float t) { return (t * t * (3.0f - 2.0f * t)); }
static float Interp(float a, float b, float t) { return (a + (b - a) * t); }
static unsigned long LookupIndex(unsigned long i) { return IndexTable[i & 0xff]; }
static float DotG(unsigned long i, float x, float y, float z) { const V3& g = Grad[i & 0xff]; return g.X * x + g.Y * y + g.Z * z; }
static float Get3DNoise(const V3& point)
{
    long x_int = GFFloatToLongFloor(point.X);
    long y_int = GFFloatToLongFloor(point.Y);
    long z_int = GFFloatToLongFloor(point.Z);
    float v0_x = point.X - x_int, v0_y = point.Y - y_int, v0_z = point.Z - z_int;
    float v1_x = v0_x - 1.0f, v1_y = v0_y - 1.0f, v1_z = v0_z - 1.0f;
    unsigned long b00 = LookupIndex(LookupIndex(x_int) + y_int);
    unsigned long b10 = LookupIndex(LookupIndex(x_int + 1) + y_int);
    unsigned long b01 = LookupIndex(LookupIndex(x_int) + y_int + 1);
    unsigned long b11 = LookupIndex(LookupIndex(x_int + 1) + y_int + 1);
    float tx = SCurve(v0_x), ty = SCurve(v0_y), tz = SCurve(v0_z);
    float r0 = Interp(Interp(DotG(b00 + z_int, v0_x, v0_y, v0_z), DotG(b10 + z_int, v1_x, v0_y, v0_z), tx),
                      Interp(DotG(b01 + z_int, v0_x, v1_y, v0_z), DotG(b11 + z_int, v1_x, v1_y, v0_z), tx), ty);
    float r1 = Interp(Interp(DotG(b00 + z_int + 1, v0_x, v0_y, v1_z), DotG(b10 + z_int + 1, v1_x, v0_y, v1_z), tx),
                      Interp(DotG(b01 + z_int + 1, v0_x, v1_y, v1_z), DotG(b11 + z_int + 1, v1_x, v1_y, v1_z), tx), ty);
    return Interp(r0, r1, tz);
}
static double Ref(double x, double y, double z)
{
    long xi = (long)floor(x), yi = (long)floor(y), zi = (long)floor(z);
    double f[3] = { x - xi, y - yi, z - zi }, s[3];
    for (int k = 0; k < 3; ++k) s[k] = f[k] * f[k] * (3 - 2 * f[k]);
    double v[2][2][2];
    for (int i = 0; i < 2; ++i) for (int j = 0; j < 2; ++j) for (int k = 0; k < 2; ++k) {
        unsigned long c = (IndexTable[(IndexTable[(xi + i) & 255] + yi + j) & 255] + zi + k) & 255;
        v[i][j][k] = Grad[c].X * (f[0] - i) + Grad[c].Y * (f[1] - j) + Grad[c].Z * (f[2] - k);
    }
    double r[2];
    for (int k = 0; k < 2; ++k) {
        double a = v[0][0][k] + (v[1][0][k] - v[0][0][k]) * s[0], b = v[0][1][k] + (v[1][1][k] - v[0][1][k]) * s[0];
        r[k] = a + (b - a) * s[1];
    }
    return r[0] + (r[1] - r[0]) * s[2];
}
int main()
{
    for (int i = 0; i < 256; ++i) {
        IndexTable[i] = (unsigned long)((i * 89 + 7) & 255);
        Grad[i].X = (float)((i * 37) % 101) / 50.0f - 1.0f; Grad[i].Y = (float)((i * 53) % 97) / 48.0f - 1.0f; Grad[i].Z = (float)((i * 71) % 89) / 44.0f - 1.0f;
    }
    static const float pts[][3] = { {0.25f, 0.75f, 0.5f}, {3.0f, 5.0f, -2.0f}, {-2.5f, 7.125f, 1.75f}, {-0.0f, -1.0f, 0.3f}, {12.9f, -3.3f, 8.6f}, {255.5f, 256.25f, -255.75f}, {-7.75f, -0.125f, 33.3f} };
    int fails = 0;
    for (unsigned k = 0; k < sizeof(pts) / sizeof(pts[0]); ++k) {
        V3 p = { pts[k][0], pts[k][1], pts[k][2] };
        float got = Get3DNoise(p); double want = Ref(p.X, p.Y, p.Z);
        if (fabs(got - want) > 1e-5) { ++fails; printf("pt %u: %f v %f\n", k, got, want); }
    }
    V3 q = { 3.0f, 5.0f, -2.0f };                            // a lattice point: every offset is 0 or -1, the blend weight 0 -> 0
    if (Get3DNoise(q) != 0.0f) ++fails;
    printf(fails ? "00f361d0_TEST FAIL\n" : "00f361d0_TEST PASS\n");
    return fails ? 1 : 0;
}