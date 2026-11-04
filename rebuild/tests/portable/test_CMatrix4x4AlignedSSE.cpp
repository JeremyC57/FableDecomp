// test_CMatrix4x4AlignedSSE.cpp -- dual-path test of engine/CMatrix4x4AlignedSSE.h (the CMatrix4x4 Aligned* SSE kernels).
// Inputs are small dyadic values, so every product and sum is exact in float: the SSE kernel (single precision), the
// portable twin and the reference loop below agree bit for bit. Build modes (tools/decomp_pipeline/portable_twins_check.py):
//   VC7.1 (no define)               -- the asm macros against the reference
//   VC7.1 /DFABLE_DUAL_PATH_TEST    -- the asm macros AND fable_portable::Aligned*: both must equal the reference
//   VC7.1 or g++ /DFABLE_PORTABLE   -- the portable expansion only
#include <stdio.h>
#include "engine/CMatrix4x4AlignedSSE.h"

#if defined(_MSC_VER)
struct __declspec(align(16)) TestMatrix { float m[16]; };
struct __declspec(align(16)) TestVector { float v[4]; };
#else
struct __attribute__((aligned(16))) TestMatrix { float m[16]; };
struct __attribute__((aligned(16))) TestVector { float v[4]; };
#endif

static float Input(int i, int k) { return (float)(((i * 7 + k * 3) % 11) - 5) * 0.25f; }

static int Check(const char* what, const float* got, const float* want, int n)
{
    int bad = 0;
    for (int i = 0; i < n; ++i)
        if (got[i] != want[i]) {
            printf("  %s[%d]: %.9g, expected %.9g\n", what, i, got[i], want[i]);
            ++bad;
        }
    return bad;
}

int main()
{
    int bad = 0;
    for (int t = 0; t < 4; ++t) {
        TestMatrix a, b, dst, m1, m2, want;
        TestVector s;
        for (int i = 0; i < 16; ++i) { a.m[i] = Input(i, t); b.m[i] = Input(i + 5, t + 1); }
        for (int c = 0; c < 4; ++c) s.v[c] = Input(c + 9, t) + 0.5f;
        // dst = a * b
        for (int r = 0; r < 4; ++r)
            for (int c = 0; c < 4; ++c)
                want.m[r * 4 + c] = (a.m[r * 4 + 0] * b.m[0 * 4 + c] + a.m[r * 4 + 1] * b.m[1 * 4 + c]) +
                                    (a.m[r * 4 + 2] * b.m[2 * 4 + c] + a.m[r * 4 + 3] * b.m[3 * 4 + c]);
        {
            TestMatrix* pdst = &dst;
            const TestMatrix* pa = &a;
            const TestMatrix* pb = &b;
            GF_SSE_MATRIX_MULTIPLY(pdst, pa, pb);
        }
        bad += Check("multiply", dst.m, want.m, 16);
        // rows 0..2 times s (transposed prescale), row 3 untouched
        for (int i = 0; i < 16; ++i) { m1.m[i] = a.m[i]; m2.m[i] = a.m[i]; want.m[i] = a.m[i]; }
        for (int r = 0; r < 3; ++r)
            for (int c = 0; c < 4; ++c)
                want.m[r * 4 + c] = a.m[r * 4 + c] * s.v[c];
        {
            TestMatrix* p = &m1;
            GF_SSE_MATRIX_PRESCALE_TRANSPOSED(p, s);
        }
        bad += Check("prescale_transposed", m1.m, want.m, 16);
        // rows 0..2 times s[r] (prescale)
        for (int r = 0; r < 3; ++r)
            for (int c = 0; c < 4; ++c)
                want.m[r * 4 + c] = a.m[r * 4 + c] * s.v[r];
        {
            TestMatrix* p = &m2;
            GF_SSE_MATRIX_PRESCALE(p, s);
        }
        bad += Check("prescale", m2.m, want.m, 16);
#if defined(FABLE_DUAL_PATH_TEST) && !defined(FABLE_PORTABLE)
        {
            TestMatrix twin, t1, t2;
            for (int i = 0; i < 16; ++i) { t1.m[i] = a.m[i]; t2.m[i] = a.m[i]; }
            fable_portable::AlignedMatrixMultiply(twin.m, a.m, b.m);
            bad += Check("twin multiply", twin.m, dst.m, 16);
            fable_portable::AlignedRowsTimesVector(t1.m, s.v);
            bad += Check("twin prescale_transposed", t1.m, m1.m, 16);
            fable_portable::AlignedRowsTimesBroadcast(t2.m, s.v);
            bad += Check("twin prescale", t2.m, m2.m, 16);
        }
#endif
    }
#if defined(FABLE_DUAL_PATH_TEST) && !defined(FABLE_PORTABLE)
    const char* mode = "asm+portable";
#elif defined(FABLE_PORTABLE)
    const char* mode = "portable";
#else
    const char* mode = "asm";
#endif
    printf("test_CMatrix4x4AlignedSSE %s: %s\n", mode, bad ? "FAIL" : "PASS");
    return bad ? 1 : 0;
}
