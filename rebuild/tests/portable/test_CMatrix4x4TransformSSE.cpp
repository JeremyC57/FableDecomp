// test_CMatrix4x4TransformSSE.cpp -- dual-path test of engine/CMatrix4x4TransformSSE.h
// (GF_SSE_MATRIX_ALIGNED_TRANSFORM_TRANSPOSED_ASSUME_W): 1 and 2 passes of four aligned vectors, out of place and in place,
// dyadic values (exact in float), against the element formula and (dual) the twin.
#include <stdio.h>
#include <string.h>
#include "engine/CMatrix4x4TransformSSE.h"
#include "fable_test_mode.h"
struct FABLE_TEST_ALIGN16 M44 { float m[16]; };
struct FABLE_TEST_ALIGN16 V4 { float X, Y, Z, W; };
static void Kernel(const M44* self, unsigned long blocks, const V4* src, V4* dest, float w)
{
    M44 expandedRows[4];
    V4 tempRows[4];
    V4 wwww; wwww.X = wwww.Y = wwww.Z = wwww.W = w;
    M44* expanded = expandedRows;
    V4* temp = tempRows;
    GF_SSE_MATRIX_ALIGNED_TRANSFORM_TRANSPOSED_ASSUME_W(self, expanded, blocks, src, dest, temp, wwww);
}
int main()
{
    int bad = 0;
    M44 m; V4 in[8], out[8], ref[8];
    const float w = 0.5f;
    for (int k = 0; k < 16; ++k) m.m[k] = (float)k * 0.25f - 1.5f;
    for (int pass = 0; pass < 4; ++pass) {
        unsigned long blocks = (unsigned long)(pass & 1) + 1;
        int inPlace = pass >> 1;
        for (int i = 0; i < 8; ++i) {
            in[i].X = (float)i * 0.5f + 1.0f; in[i].Y = 2.0f - (float)i; in[i].Z = (float)(i * i) * 0.25f; in[i].W = 77.0f;
            out[i].X = out[i].Y = out[i].Z = out[i].W = -9.0f;
            ref[i] = inPlace ? in[i] : out[i];
            if ((unsigned long)i < blocks * 4) {
                for (int r = 0; r < 3; ++r)
                    (&ref[i].X)[r] = (m.m[r * 4] * in[i].X + m.m[r * 4 + 1] * in[i].Y) + (m.m[r * 4 + 2] * in[i].Z + m.m[r * 4 + 3] * w);
                ref[i].W = w;
            }
        }
        V4* dest = inPlace ? in : out;
#ifdef FABLE_TEST_DUAL
        V4 tin[8], tout[8]; memcpy(tin, in, sizeof in); memcpy(tout, out, sizeof out);
        V4* tdest = inPlace ? tin : tout;
        float ex[64], tmp[16]; V4 ww; ww.X = ww.Y = ww.Z = ww.W = w;
        fable_portable::MatrixAlignedTransformTransposedAssumeWSSE(m.m, ex, blocks, &tin[0].X, &tdest[0].X, tmp, &ww.X);
#endif
        Kernel(&m, blocks, in, dest, w);
        if (memcmp(dest, ref, sizeof ref)) { printf("  pass %d differs\n", pass); ++bad; }
#ifdef FABLE_TEST_DUAL
        if (memcmp(dest, tdest, sizeof ref)) { printf("  pass %d twin differs\n", pass); ++bad; }
#endif
    }
    printf("test_CMatrix4x4TransformSSE %s: %s\n", kFableTestMode, bad ? "FAIL" : "PASS");
    return bad ? 1 : 0;
}
