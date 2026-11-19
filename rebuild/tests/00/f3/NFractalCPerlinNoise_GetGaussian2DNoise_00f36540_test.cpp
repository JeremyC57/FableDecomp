// Behaviour test for NFractal::CPerlinNoise::GetGaussian2DNoise (0x00F36540): the row's logic on stand-alone tables, floored with the
// shared GFFloatToLongFloor (the retail asm under VC7.1, its FABLE_PORTABLE twin elsewhere), checked against floor() / smoothstep.
#ifndef _MSC_VER
#define FABLE_PORTABLE
#endif
#include <stdio.h>
#include <math.h>
#include "engine/GFFloatToLongFloor.h"
struct V2 { float X, Y; };
static unsigned long IndexTable[256];
static float VectorX[256];
static float Interp(float old_val, float new_val, float interp) { float delta = (new_val - old_val) * interp; return old_val + delta; }
static float GetGaussian2DNoise(const V2& point)
{
    long x_int = GFFloatToLongFloor(point.X);
    long y_int = GFFloatToLongFloor(point.Y);
    float v0_x = point.X - x_int;
    float v0_y = point.Y - y_int;
    float tx = (3.0f - (2.0f * v0_x)) * v0_x * v0_x;
    float ty = (3.0f - (2.0f * v0_y)) * v0_y * v0_y;
    unsigned long b00 = IndexTable[(IndexTable[x_int & 0xff] + y_int) & 0xff];
    unsigned long b01 = IndexTable[(IndexTable[x_int & 0xff] + y_int + 1) & 0xff];
    unsigned long b10 = IndexTable[(IndexTable[(x_int + 1) & 0xff] + y_int) & 0xff];
    unsigned long b11 = IndexTable[(IndexTable[(x_int + 1) & 0xff] + y_int + 1) & 0xff];
    return Interp(Interp(VectorX[b00 & 0xff], VectorX[b10 & 0xff], tx), Interp(VectorX[b01 & 0xff], VectorX[b11 & 0xff], tx), ty);
}
static float Ref(double x, double y)
{
    long xi = (long)floor(x), yi = (long)floor(y);
    double fx = x - xi, fy = y - yi, sx = fx * fx * (3 - 2 * fx), sy = fy * fy * (3 - 2 * fy);
    double c00 = VectorX[IndexTable[(IndexTable[xi & 255] + yi) & 255] & 255], c01 = VectorX[IndexTable[(IndexTable[xi & 255] + yi + 1) & 255] & 255];
    double c10 = VectorX[IndexTable[(IndexTable[(xi + 1) & 255] + yi) & 255] & 255], c11 = VectorX[IndexTable[(IndexTable[(xi + 1) & 255] + yi + 1) & 255] & 255];
    double a = c00 + (c10 - c00) * sx, b = c01 + (c11 - c01) * sx;
    return (float)(a + (b - a) * sy);
}
int main()
{
    for (int i = 0; i < 256; ++i) { IndexTable[i] = (unsigned long)((i * 167 + 13) & 255); VectorX[i] = (float)((i * 37) % 101) / 50.0f - 1.0f; }
    static const float pts[][2] = { {0.25f, 0.75f}, {3.0f, 5.0f}, {-2.5f, 7.125f}, {-0.0f, -1.0f}, {12.9f, -3.3f}, {255.5f, 256.25f}, {1.0f, 0.5f} };
    int fails = 0;
    for (unsigned k = 0; k < sizeof(pts) / sizeof(pts[0]); ++k) {
        V2 p = { pts[k][0], pts[k][1] };
        float got = GetGaussian2DNoise(p), want = Ref(p.X, p.Y);
        if (fabs(got - want) > 1e-5f) { ++fails; printf("pt %u: %f v %f\n", k, got, want); }
    }
    // integer lattice points return the corner value exactly (the floor fix-up path)
    V2 q = { 3.0f, 5.0f };
    if (GetGaussian2DNoise(q) != VectorX[IndexTable[(IndexTable[3] + 5) & 255] & 255]) ++fails;
    printf(fails ? "00f36540_TEST FAIL\n" : "00f36540_TEST PASS\n");
    return fails ? 1 : 0;
}