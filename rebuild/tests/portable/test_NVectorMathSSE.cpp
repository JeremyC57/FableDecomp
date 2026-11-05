// test_NVectorMathSSE.cpp -- dual-path test of engine/NVectorMathSSE.h: the five NVectorMath SSE kernels on 16-byte aligned
// C4DVector arrays with byte strides of 16 and 32, weights 0, 1/4, 1/2, 3/4, 1 and small dyadic coordinates (every product
// and sum exact in float), against a reference loop and (dual) the twins. count 0 must leave the output alone.
#include <stdio.h>
#include <string.h>
#include "engine/NVectorMathSSE.h"
#include "fable_test_mode.h"
struct FABLE_TEST_ALIGN16 V4 { float v[4]; };
static const int kN = 12;
static void Fill(V4* a, float base) { for (int i = 0; i < kN; ++i) for (int l = 0; l < 4; ++l) a[i].v[l] = base + (float)(i * 4 + l) * 0.25f - 3.0f; }
static int Same(const V4* a, const V4* b, const char* what)
{
    if (memcmp(a, b, sizeof(V4) * kN) == 0) return 0;
    printf("  %s differs\n", what); return 1;
}
static void K1(V4* out, const V4* from, unsigned long count, float t, unsigned long outStride, unsigned long fromStride)
{
    FABLE_TEST_ALIGN16 float tt[4] = { t, 0, 0, 0 };
    FABLE_TEST_ALIGN16 float oneMinusT[4] = { 1.0f - t, 0, 0, 0 };
    GF_SSE_INTERPOLATE_VECTORS(out, from, count, oneMinusT, tt, outStride, fromStride);
}
static void K2(V4* out, const V4* from, const V4* to, unsigned long count, float t, unsigned long outStride,
               unsigned long fromStride, unsigned long toStride)
{
    FABLE_TEST_ALIGN16 V4 w = { { t, 0, 0, 0 } };
    FABLE_TEST_ALIGN16 V4 oneMinusW = { { 1.0f - t, 0, 0, 0 } };
    GF_SSE_INTERPOLATE_VECTORS_FROM_TO(out, from, to, count, oneMinusW, w, outStride, fromStride, toStride);
}
static void K3(V4* out, const V4* from, const V4* to, const float* weights, unsigned long count, unsigned long outStride,
               unsigned long fromStride, unsigned long toStride, unsigned long weightStride)
{
    FABLE_TEST_ALIGN16 V4 one = { { 1.0f, 1.0f, 1.0f, 1.0f } };
    GF_SSE_INTERPOLATE_VECTORS_WEIGHTED(out, from, to, weights, count, one, outStride, fromStride, toStride, weightStride);
}
static void K4(V4* out, const V4* in, const V4* weights, unsigned long count, unsigned long weightStride,
               unsigned long inStride, unsigned long outStride)
{
    GF_SSE_FINISH_PARTIAL_SLERP(out, in, weights, count, weightStride, inStride, outStride);
}
static void K5(const V4* value, V4* dst, unsigned long count) {
    GF_SSE_CLEAR_4D_VECTORS(value, dst, count);
}
int main()
{
    int bad = 0;
    static const float ts[] = { 0.0f, 0.25f, 0.5f, 0.75f, 1.0f };
    FABLE_TEST_ALIGN16 V4 a[kN], b[kN], c[kN], r[kN], w[kN];
    for (int ti = 0; ti < 5; ++ti) for (unsigned long st = 16; st <= 32; st += 16) for (unsigned long count = 0; count <= 6; count += 3) {
        float t = ts[ti];
        // 1: out = out*(1-t) + from*t
        Fill(a, 1.0f); Fill(b, 2.5f); memcpy(r, a, sizeof a);
        K1(a, b, count, t, st, st);
        for (unsigned long i = 0; i < count; ++i) for (int l = 0; l < 4; ++l) { V4& o = r[i * st / 16]; o.v[l] = o.v[l] * (1.0f - t) + b[i * st / 16].v[l] * t; }
        bad += Same(a, r, "INTERPOLATE_VECTORS");
#ifdef FABLE_TEST_DUAL
        { FABLE_TEST_ALIGN16 V4 q[kN]; Fill(q, 1.0f); float om = 1.0f - t; fable_portable::InterpolateVectorsSSE(q, b, count, &om, &t, st, st); bad += Same(a, q, "twin 1"); }
#endif
        // 2: out = from*(1-t) + to*t
        Fill(a, 0.0f); Fill(b, 1.0f); Fill(c, -2.0f); memcpy(r, a, sizeof a);
        K2(a, b, c, count, t, st, st, st);
        for (unsigned long i = 0; i < count; ++i) for (int l = 0; l < 4; ++l) r[i * st / 16].v[l] = b[i * st / 16].v[l] * (1.0f - t) + c[i * st / 16].v[l] * t;
        bad += Same(a, r, "INTERPOLATE_VECTORS_FROM_TO");
#ifdef FABLE_TEST_DUAL
        { FABLE_TEST_ALIGN16 V4 q[kN]; Fill(q, 0.0f); float om = 1.0f - t; fable_portable::InterpolateVectorsFromToSSE(q, b, c, count, &om, &t, st, st, st); bad += Same(a, q, "twin 2"); }
#endif
        // 3: per-vector weights (the weight stream at 16-byte steps: weights[i*4])
        float wts[4 * kN];
        for (int i = 0; i < 4 * kN; ++i) wts[i] = ts[(i / 4 + ti) % 5];
        Fill(a, 0.0f); Fill(b, 1.0f); Fill(c, -2.0f); memcpy(r, a, sizeof a);
        K3(a, b, c, wts, count, st, st, st, 16);
        for (unsigned long i = 0; i < count; ++i) for (int l = 0; l < 4; ++l) { float x = wts[i * 4]; r[i * st / 16].v[l] = b[i * st / 16].v[l] * (1.0f - x) + c[i * st / 16].v[l] * x; }
        bad += Same(a, r, "INTERPOLATE_VECTORS_WEIGHTED");
#ifdef FABLE_TEST_DUAL
        { FABLE_TEST_ALIGN16 V4 q[kN]; Fill(q, 0.0f); FABLE_TEST_ALIGN16 float one[4] = { 1, 1, 1, 1 }; fable_portable::InterpolateVectorsWeightedSSE(q, b, c, wts, count, one, st, st, st, 16); bad += Same(a, q, "twin 3"); }
#endif
        // 4: out = out*w0 + in*w1 (weight pairs)
        for (int i = 0; i < kN; ++i) { w[i].v[0] = ts[(i + ti) % 5]; w[i].v[1] = 1.0f - ts[(i + ti) % 5]; w[i].v[2] = 9.0f; w[i].v[3] = 9.0f; }
        Fill(a, 3.0f); Fill(b, -1.0f); memcpy(r, a, sizeof a);
        K4(a, b, w, count, st, st, st);
        for (unsigned long i = 0; i < count; ++i) for (int l = 0; l < 4; ++l) { V4& o = r[i * st / 16]; o.v[l] = o.v[l] * w[i * st / 16].v[0] + b[i * st / 16].v[l] * w[i * st / 16].v[1]; }
        bad += Same(a, r, "FINISH_PARTIAL_SLERP");
#ifdef FABLE_TEST_DUAL
        { FABLE_TEST_ALIGN16 V4 q[kN]; Fill(q, 3.0f); fable_portable::FinishPartialSlerpSSE(q, b, w, count, st, st, st); bad += Same(a, q, "twin 4"); }
#endif
        // 5: clear (unaligned source value)
        float raw[5] = { 7.0f, 1.5f, -2.25f, 0.125f, 3.0f };
        Fill(a, 0.0f); memcpy(r, a, sizeof a);
        K5((const V4*)(raw + 1), a, count);
        for (unsigned long i = 0; i < count; ++i) for (int l = 0; l < 4; ++l) r[i].v[l] = raw[1 + l];
        bad += Same(a, r, "CLEAR_4D_VECTORS");
#ifdef FABLE_TEST_DUAL
        { FABLE_TEST_ALIGN16 V4 q[kN]; Fill(q, 0.0f); fable_portable::Clear4DVectorsSSE(raw + 1, q, count); bad += Same(a, q, "twin 5"); }
#endif
    }
    printf("test_NVectorMathSSE %s: %s\n", kFableTestMode, bad ? "FAIL" : "PASS");
    return bad ? 1 : 0;
}
