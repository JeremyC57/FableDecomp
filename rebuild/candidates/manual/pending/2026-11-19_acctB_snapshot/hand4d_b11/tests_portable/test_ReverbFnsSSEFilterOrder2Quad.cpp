// test_ReverbFnsSSEFilterOrder2Quad.cpp -- dual-path test of engine/ReverbFnsSSEFilterOrder2Quad.h (the four-filter SSE
// second-order filter kernel of GameCODA CReverbFnsSSE::ApplyFilterOrder2_quad, 0x00C88E60). The coefficients are powers of two and
// the samples small integers, so every step is exact in single precision and the SSE kernel, the portable twin and the reference
// below agree bit for bit (the reference checks that its own intermediates are exact). Cases: 1 sample (the `dec ecx ; jne` lower
// bound), 9 samples, and an in-place run (pfDest == pfSrc); plus one INEXACT case against golden bit patterns
// (association-sensitive: it kills twin mutants that re-associate a sum). Build modes (tools/decomp_pipeline/portable_twins_check.py):
//   VC7.1 (no define)               -- the asm macro against the reference
//   VC7.1 /DFABLE_DUAL_PATH_TEST    -- the asm macro AND fable_portable::ApplyFilterOrder2QuadSSE against each other
//   VC7.1 or g++ /DFABLE_PORTABLE   -- the portable expansion only
#include <stdio.h>
#include <string.h>
#include "engine/ReverbFnsSSEFilterOrder2Quad.h"

#if defined(_MSC_VER)
#define TEST_ALIGN16 __declspec(align(16))
#else
#define TEST_ALIGN16 __attribute__((aligned(16)))
#endif

struct TEST_ALIGN16 TestBlock { float v[40]; };
static const unsigned int kLane = 128;

static void SetupData(TestBlock* d)
{
    static const float A1[4] = { 0.5f, 0.25f, -0.5f, -0.25f }, A2[4] = { -0.25f, 0.5f, 0.25f, -0.5f };
    static const float B0[4] = { 1.0f, 2.0f, 0.5f, -1.0f }, B1[4] = { -0.5f, 0.25f, 1.0f, 0.5f }, B2[4] = { 0.25f, -0.25f, 0.5f, 2.0f };
    static const float G[4] = { 0.5f, 1.0f, 2.0f, 0.25f }, H0[4] = { 1.0f, -3.0f, 2.5f, 4.0f }, H1[4] = { -2.0f, 1.5f, 3.0f, -0.5f };
    for (int l = 0; l < 4; ++l) {
        d->v[0 + l] = A1[l]; d->v[4 + l] = A2[l]; d->v[8 + l] = 1.0f - (A1[l] + A2[l]);
        d->v[12 + l] = B0[l]; d->v[16 + l] = B1[l]; d->v[20 + l] = B2[l]; d->v[24 + l] = -(B0[l] + B1[l] + B2[l]);
        d->v[28 + l] = H0[l]; d->v[32 + l] = H1[l]; d->v[36 + l] = G[l];
    }
}

static void SetupSamples(float* src, float* dst)
{
    for (unsigned int k = 0; k < 4 * kLane + 4; ++k) { src[k] = (float)((int)((k * 37) % 11) - 5); dst[k] = -1000.0f - (float)k; }
}

static int g_inexact = 0;
static float Exact(double v) { float f = (float)v; if ((double)f != v) ++g_inexact; return f; }

// Reference: the kernel's operation order, each step checked to be exact in single precision.
static void Reference(float* dst, const float* src, unsigned int n, TestBlock* d)
{
    for (int l = 0; l < 4; ++l) {
        double h0 = d->v[28 + l], h1 = d->v[32 + l];
        for (unsigned int s = 0; s < n; ++s) {
            double fb = Exact(Exact(d->v[4 + l] * h1) + Exact(d->v[0 + l] * h0));
            double ff = Exact(Exact(d->v[20 + l] * h1) + Exact(d->v[16 + l] * h0));
            double w = Exact(Exact(src[l * kLane + s] + (double)d->v[8 + l]) + fb);
            double y = Exact(Exact(Exact(Exact(d->v[12 + l] * w) + ff) + d->v[24 + l]) * d->v[36 + l]);
            h1 = h0; h0 = w;
            dst[l * kLane + s] = (float)y;
        }
        d->v[28 + l] = (float)h0; d->v[32 + l] = (float)h1;
    }
}

static void RunKernel(float* dstBase, const float* srcBase, unsigned int count, TestBlock* block)
{
    float* pfDest = dstBase;
    const float* pfSrc = srcBase;
    unsigned int uiSamples = count;
    float* pfData = block->v;
    CODA_SSE_APPLY_FILTER_ORDER2_QUAD(pfDest, pfSrc, uiSamples, pfData);
}

static int Check(const char* what, const float* got, const float* want, int n)
{
    int bad = 0;
    for (int i = 0; i < n; ++i)
        if (memcmp(&got[i], &want[i], sizeof(float)) != 0) {
            if (bad < 4) printf("  %s[%d]: %.9g, expected %.9g\n", what, i, got[i], want[i]);
            ++bad;
        }
    return bad;
}

static float g_src[4 * 128 + 4], g_dst[4 * 128 + 4], g_want[4 * 128 + 4], g_wsrc[4 * 128 + 4];

static int RunCase(const char* name, unsigned int n, int inPlace)
{
    int bad = 0;
    TEST_ALIGN16 TestBlock got, want;
    SetupData(&got); SetupData(&want);
    SetupSamples(g_src, g_dst); SetupSamples(g_wsrc, g_want);
    if (inPlace) { memcpy(g_dst, g_src, sizeof(g_src)); memcpy(g_want, g_wsrc, sizeof(g_wsrc)); }
    Reference(g_want, g_wsrc, n, &want);
    RunKernel(g_dst, inPlace ? g_dst : g_src, n, &got);
    char what[64];
    sprintf(what, "%s dst", name); bad += Check(what, g_dst, g_want, 4 * kLane + 4);
    sprintf(what, "%s block", name); bad += Check(what, got.v, want.v, 40);
#if defined(FABLE_DUAL_PATH_TEST) && !defined(FABLE_PORTABLE)
    {
        TEST_ALIGN16 TestBlock twin;
        static float tdst[4 * 128 + 4], tsrc[4 * 128 + 4];
        SetupData(&twin); SetupSamples(tsrc, tdst);
        if (inPlace) memcpy(tdst, tsrc, sizeof(tsrc));
        fable_portable::ApplyFilterOrder2QuadSSE(tdst, inPlace ? tdst : tsrc, n, twin.v);
        sprintf(what, "%s twin dst", name); bad += Check(what, tdst, g_dst, 4 * kLane + 4);
        sprintf(what, "%s twin block", name); bad += Check(what, twin.v, got.v, 40);
    }
#endif
    return bad;
}


// Inexact case: non-dyadic coefficients and samples, so each association order gives different bits. Golden bit patterns from an
// independent IEEE single-precision computation (numpy float32, one rounding per SSE operation, the kernel's order). The asm path is
// checked against them in every VC7.1 mode; the portable twin only where float is evaluated in single precision (not on x87 VC7.1).
static const unsigned int kInexactBlock[40] = {
    0x3EC00000u, 0x3E99999Au, 0xBF333333u, 0x3DCCCCCDu, 0xBE400000u, 0x3E4CCCCDu, 0x3E19999Au,
    0xBEA8F5C3u, 0x3F500000u, 0x3F000000u, 0x3FC66666u, 0x3F9D70A4u, 0x3F8CCCCDu, 0x3F666666u,
    0xBFA66666u, 0x3F333333u, 0xBE800000u, 0x3EBD70A4u, 0x3F1C28F6u, 0xBE4CCCCDu, 0x3E800000u,
    0xBDE147AEu, 0x3D4CCCCDu, 0x3EE66666u, 0xBF8CCCCDu, 0xBF947AE1u, 0x3F23D709u, 0xBF733333u,
    0x40000000u, 0xBFD9999Au, 0x3E99999Au, 0x40533333u, 0xBFC00000u, 0x3F666666u, 0xC00CCCCDu,
    0x3ECCCCCDu, 0x3F400000u, 0x3FA66666u, 0x3F19999Au, 0x40066666u,
};
static const unsigned int kInexactSrc[28] = {    // lane l sample s at [l*7 + s]
    0xBFECCCCDu, 0xBEBD70A4u, 0x3F8E147Bu, 0xBFBD70A4u, 0x00000000u, 0x3FBD70A4u, 0xBF8E147Bu,
    0x3EBD70A4u, 0x3FECCCCDu, 0xBF3D70A4u, 0x3F3D70A4u, 0xBFECCCCDu, 0xBEBD70A4u, 0x3F8E147Bu,
    0xBFBD70A4u, 0x00000000u, 0x3FBD70A4u, 0xBF8E147Bu, 0x3EBD70A4u, 0x3FECCCCDu, 0xBF3D70A4u,
    0x3F3D70A4u, 0xBFECCCCDu, 0xBEBD70A4u, 0x3F8E147Bu, 0xBFBD70A4u, 0x00000000u, 0x3FBD70A4u,
};
static const unsigned int kInexactOut[28] = {    // lane l sample s at [l*7 + s]
    0xBFBE4290u, 0xBECA472Du, 0x3F44CFF6u, 0xBF916C84u, 0xBDAD33B8u, 0x3F8FF9BDu, 0xBF416E50u,
    0xBFE94AF4u, 0x3FC49E44u, 0x3D8A2014u, 0x3F12A05Au, 0xBFC23AEDu, 0xBFE276A0u, 0x3F0D0FC1u,
    0x3F4B5DCEu, 0xBFA3CE87u, 0xBE3C3BD4u, 0x3FAC0E8Fu, 0xBFCF3A63u, 0xBBC3B44Du, 0x3F836C12u,
    0x3E3C5FF9u, 0xBFFD841Bu, 0x3F2C7342u, 0x3F4282E1u, 0xC048D132u, 0x3F8D9297u, 0x3FF62710u,
};
static const unsigned int kInexactFinal[40] = {
    0x3EC00000u, 0x3E99999Au, 0xBF333333u, 0x3DCCCCCDu, 0xBE400000u, 0x3E4CCCCDu, 0x3E19999Au,
    0xBEA8F5C3u, 0x3F500000u, 0x3F000000u, 0x3FC66666u, 0x3F9D70A4u, 0x3F8CCCCDu, 0x3F666666u,
    0xBFA66666u, 0x3F333333u, 0xBE800000u, 0x3EBD70A4u, 0x3F1C28F6u, 0xBE4CCCCDu, 0x3E800000u,
    0xBDE147AEu, 0x3D4CCCCDu, 0x3EE66666u, 0xBF8CCCCDu, 0xBF947AE1u, 0x3F23D709u, 0xBF733333u,
    0x3F0968E4u, 0x3FC7418Du, 0x3D0F4680u, 0x402E9E9Bu, 0x401D4D12u, 0x3E92EC0Eu, 0x3FD0B39Bu,
    0x3E990CE6u, 0x3F400000u, 0x3FA66666u, 0x3F19999Au, 0x40066666u,
};
#if defined(_MSC_VER) && defined(_M_IX86)
static const int kTwinSinglePrecision = 0;   // VC7.1 (dual and portable builds): x87 keeps the twin's intermediates in extended precision
#else
static const int kTwinSinglePrecision = 1;
#endif

static int RunInexact(int twin)
{
    int bad = 0;
    TEST_ALIGN16 TestBlock blk;
    memcpy(blk.v, kInexactBlock, sizeof(blk.v));
    for (unsigned int k = 0; k < 4 * kLane + 4; ++k) { g_src[k] = 0.0f; g_dst[k] = -1000.0f - (float)k; }
    for (int l = 0; l < 4; ++l) memcpy(&g_src[l * kLane], &kInexactSrc[l * 7], 7 * sizeof(float));
    if (twin) {
#if defined(FABLE_PORTABLE) || defined(FABLE_DUAL_PATH_TEST)
        fable_portable::ApplyFilterOrder2QuadSSE(g_dst, g_src, 7, blk.v);
#endif
    } else
        RunKernel(g_dst, g_src, 7, &blk);
    float want[28], got[28];
    memcpy(want, kInexactOut, sizeof(want));
    for (int l = 0; l < 4; ++l) memcpy(&got[l * 7], &g_dst[l * kLane], 7 * sizeof(float));
    bad += Check(twin ? "inexact twin dst" : "inexact dst", got, want, 28);
    float wfin[40]; memcpy(wfin, kInexactFinal, sizeof(wfin));
    bad += Check(twin ? "inexact twin block" : "inexact block", blk.v, wfin, 40);
    return bad;
}

int main()
{
    int bad = 0;
    bad += RunCase("n1", 1, 0);
    bad += RunCase("n9", 9, 0);
    bad += RunCase("inplace7", 7, 1);
#if defined(FABLE_PORTABLE)
    if (kTwinSinglePrecision) bad += RunInexact(0);    // the macro is the twin here
#else
    bad += RunInexact(0);                               // the asm path
#if defined(FABLE_DUAL_PATH_TEST)
    if (kTwinSinglePrecision) bad += RunInexact(1);
#endif
#endif
    if (g_inexact) { printf("  reference left single precision %d times (test data must stay exact)\n", g_inexact); ++bad; }
#if defined(FABLE_DUAL_PATH_TEST) && !defined(FABLE_PORTABLE)
    const char* mode = "asm+portable";
#elif defined(FABLE_PORTABLE)
    const char* mode = "portable";
#else
    const char* mode = "asm";
#endif
    printf("test_ReverbFnsSSEFilterOrder2Quad %s: %s\n", mode, bad ? "FAIL" : "PASS");
    return bad ? 1 : 0;
}
