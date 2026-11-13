// test_CEngineClothRendererSSE.cpp -- dual-path test of engine/CEngineClothRendererSSE.h (GF_SSE_ADD_TRIANGLE_NORMAL): the face normal
// (p2 - p0) x (p1 - p0) added to three corner normals of aligned C4DVector arrays, dyadic values (exact), against the lane loop and
// (dual) the twin. For rebuild/tests/portable/ (chunk 2008 hand-back).
#include <stdio.h>
#include <string.h>
#include "engine/CEngineClothRendererSSE.h"
#include "fable_test_mode.h"
struct FABLE_TEST_ALIGN16 V4 { float v[4]; };
static void Kernel(const V4* positions, V4* normals, unsigned long index0, unsigned long index1, unsigned long index2)
{
    GF_SSE_ADD_TRIANGLE_NORMAL(positions, normals, index0, index1, index2);
}
int main()
{
    int bad = 0;
    static V4 p[4] = { { { 0.5f, 1.0f, -2.0f, 1.0f } }, { { 3.0f, -1.5f, 0.25f, 1.0f } }, { { -0.75f, 2.0f, 4.0f, 1.0f } }, { { 1.0f, 1.0f, 1.0f, 0.0f } } };
    static const unsigned long tri[3][3] = { { 0, 1, 2 }, { 2, 3, 0 }, { 1, 3, 2 } };
    for (int t = 0; t < 3; ++t) {
        V4 n[4], r[4];
        for (int k = 0; k < 4; ++k) for (int l = 0; l < 4; ++l) n[k].v[l] = r[k].v[l] = 0.125f * (k + 1) - 0.5f * l;
        Kernel(p, n, tri[t][0], tri[t][1], tri[t][2]);
        const float* a0 = p[tri[t][0]].v; const float* a1 = p[tri[t][1]].v; const float* a2 = p[tri[t][2]].v;
        float a[4], b[4], c[4];
        for (int l = 0; l < 4; ++l) { a[l] = a2[l] - a0[l]; b[l] = a1[l] - a0[l]; }
        c[0] = a[1] * b[2] - a[2] * b[1]; c[1] = a[2] * b[0] - a[0] * b[2]; c[2] = a[0] * b[1] - a[1] * b[0]; c[3] = a[0] * b[0] - a[0] * b[0];
        for (int k = 0; k < 3; ++k) for (int l = 0; l < 4; ++l) r[tri[t][k]].v[l] = r[tri[t][k]].v[l] + c[l];
        if (memcmp(n, r, sizeof n)) { printf("  triangle %d differs\n", t); ++bad; }
#ifdef FABLE_TEST_DUAL
        V4 q[4];
        for (int k = 0; k < 4; ++k) for (int l = 0; l < 4; ++l) q[k].v[l] = 0.125f * (k + 1) - 0.5f * l;
        fable_portable::AddTriangleNormalSSE(p, q, tri[t][0], tri[t][1], tri[t][2]);
        if (memcmp(n, q, sizeof n)) { printf("  triangle %d twin differs\n", t); ++bad; }
#endif
    }
    printf("test_CEngineClothRendererSSE %s: %s\n", kFableTestMode, bad ? "FAIL" : "PASS");
    return bad ? 1 : 0;
}
