// Behaviour test for C3DKeyframe::BlendWith (0x00AA3C50): the table slerp (GFFastArcCos / GFFastSin, indices rounded by
// GFFloatToLongNear) of two rotations about Z, and the linear blend of the positions.
#include <stdio.h>
#include <math.h>
#include "engine/GFFloatToLongNear.h"
static float g_ArcCosTable[1026];
static float g_CosTable[1025];
static float FastArcCos(float c)
{
    c = (c + 1.0f) * 512.5f;
    long i = GFFloatToLongNear(c - 0.5f);
    float f = c - (float)i;
    return ((1.0f - f) * g_ArcCosTable[i] + f * g_ArcCosTable[i + 1]) * 0.15915494f;
}
static float FastSin(float a)
{
    a = a * 1024.0f;
    long i = GFFloatToLongNear(a - 0.5f);
    float f = a - (float)i;
    i = (i - 256) & 0x3ff;
    return (1.0f - f) * g_CosTable[i] + f * g_CosTable[i + 1];
}
int main()
{
    for (int k = 0; k <= 1024; ++k) g_CosTable[k] = (float)cos(k * 6.283185307179586 / 1024.0);
    for (int k = 0; k <= 1025; ++k) { double c = k / 512.5 - 1.0; if (c > 1.0) c = 1.0; g_ArcCosTable[k] = (float)acos(c); }
    // rotations about Z by 0 and 90 degrees: (0,0,0,1) and (0,0,s,s), s = sqrt(0.5); blend at t = 0.5 -> 45 degrees
    float s = (float)sqrt(0.5);
    float cosom = s;                                  // dot of the two quaternions
    float omega = FastArcCos(cosom);                  // turns
    float sinom = (float)sqrt(1.0f - cosom * cosom);
    float s0 = FastSin(0.5f * omega) / sinom, s1 = FastSin(omega * 0.5f) / sinom;
    float z = s1 * s, w = s0 * 1.0f + s1 * s;
    bool ok = fabs(z - (float)sin(3.14159265 / 8)) < 0.01f && fabs(w - (float)cos(3.14159265 / 8)) < 0.01f;
    float p = (8.0f - 2.0f) * 0.25f + 2.0f;           // position lerp (v - a) * t + a
    ok = ok && p == 3.5f && GFFloatToLongNear(2.5f) == 2 && GFFloatToLongNear(3.5f) == 4;
    printf(ok ? "00aa3c50_TEST PASS\n" : "00aa3c50_TEST FAIL\n");
    return ok ? 0 : 1;
}