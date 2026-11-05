// test_CParticleProgramSSE.cpp -- dual-path test of engine/CParticleProgramSSE.h (GF_SSE_ADD_WEIGHTED_VECTOR): target +=
// source * weight.X on aligned vectors, dyadic values (exact), against the lane loop and (dual) the twin.
#include <stdio.h>
#include <string.h>
#include "engine/CParticleProgramSSE.h"
#include "fable_test_mode.h"
struct FABLE_TEST_ALIGN16 V4 { float v[4]; };
static void Kernel(const V4* source, V4* target, float w)
{
    V4 weight; weight.v[0] = w; weight.v[1] = weight.v[2] = weight.v[3] = 0.0f;
    GF_SSE_ADD_WEIGHTED_VECTOR(source, target, weight);
}
int main()
{
    int bad = 0;
    static const float ws[] = { 0.0f, 0.125f, 0.5f, 1.0f, -0.75f, 2.0f };
    for (int i = 0; i < 6; ++i) {
        V4 s = { { 1.5f, -2.0f, 0.25f, 8.0f } }, t = { { -3.0f, 0.5f, 4.0f, 1.0f } }, r = t;
        Kernel(&s, &t, ws[i]);
        for (int l = 0; l < 4; ++l) r.v[l] = r.v[l] + s.v[l] * ws[i];
        if (memcmp(&t, &r, sizeof t)) { printf("  w=%g differs\n", ws[i]); ++bad; }
#ifdef FABLE_TEST_DUAL
        V4 q = { { -3.0f, 0.5f, 4.0f, 1.0f } }; fable_portable::AddWeightedVectorSSE(s.v, q.v, ws[i]);
        if (memcmp(&t, &q, sizeof t)) { printf("  w=%g twin differs\n", ws[i]); ++bad; }
#endif
    }
    printf("test_CParticleProgramSSE %s: %s\n", kFableTestMode, bad ? "FAIL" : "PASS");
    return bad ? 1 : 0;
}
