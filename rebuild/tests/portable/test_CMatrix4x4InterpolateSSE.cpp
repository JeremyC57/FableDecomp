// test_CMatrix4x4InterpolateSSE.cpp -- dual-path test of engine/CMatrix4x4InterpolateSSE.h (GF_SSE_MATRIX_SET_INTERPOLATE):
// self = a*(1-t) + b*t on aligned 4x4 float matrices, t in {0, 1/4, 1/2, 3/4, 1}, dyadic elements (exact in float), against
// the element loop and (dual) the twin.
#include <stdio.h>
#include <string.h>
#include "engine/CMatrix4x4InterpolateSSE.h"
#include "fable_test_mode.h"
struct FABLE_TEST_ALIGN16 M44 { float m[16]; };
struct FABLE_TEST_ALIGN16 V4 { float X, Y, Z, W; };
static void Kernel(M44* self, const M44& a, const M44& b, float t)
{
    V4 oneMinusT; oneMinusT.X = 1.0f - t; oneMinusT.Y = oneMinusT.Z = oneMinusT.W = 0.0f;
    V4 weight; weight.X = t; weight.Y = weight.Z = weight.W = 0.0f;
    GF_SSE_MATRIX_SET_INTERPOLATE(a, b, self, oneMinusT, weight);
}
int main()
{
    int bad = 0;
    static const float ts[] = { 0.0f, 0.25f, 0.5f, 0.75f, 1.0f };
    M44 a, b, s, r;
    for (int k = 0; k < 16; ++k) { a.m[k] = (float)k * 0.5f - 4.0f; b.m[k] = 6.0f - (float)k * 0.25f; }
    for (int i = 0; i < 5; ++i) {
        float t = ts[i];
        memset(&s, 0, sizeof s);
        Kernel(&s, a, b, t);
        for (int k = 0; k < 16; ++k) r.m[k] = a.m[k] * (1.0f - t) + b.m[k] * t;
        if (memcmp(&s, &r, sizeof s)) { printf("  t=%g differs\n", t); ++bad; }
#ifdef FABLE_TEST_DUAL
        M44 q; fable_portable::MatrixSetInterpolateSSE(a.m, b.m, q.m, 1.0f - t, t);
        if (memcmp(&s, &q, sizeof s)) { printf("  t=%g twin differs\n", t); ++bad; }
#endif
    }
    printf("test_CMatrix4x4InterpolateSSE %s: %s\n", kFableTestMode, bad ? "FAIL" : "PASS");
    return bad ? 1 : 0;
}
