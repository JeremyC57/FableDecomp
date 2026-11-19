// test_ReverbFnsSSEFilterOrder2Quad.cpp -- dual-path test of CReverbFnsSSE::ApplyFilterOrder2_quad (0x00C88E60), a library row
// (toolchain_library: byte-identical to GameCODA 2.10 CODAApi.lib member ReverbFns_SSE.obj) and its FABLE_PORTABLE twin
// engine/ReverbFnsSSEFilterOrder2Quad.h. Build modes (tools/decomp_pipeline/portable_twins_check.py):
//   VC7.1 (no define)              -- retail side: the CODAApi.lib member (LIB must hold GameCODA 2.10 CODA/lib/Win32, e.g.
//                                     FABLE_VC71_LIB=<that dir>; the link fails without it, so the retail side never goes quiet)
//   VC7.1 /DFABLE_DUAL_PATH_TEST   -- the library member AND the twin kernel on the same blocks, bit for bit
//   VC7.1 or g++ /DFABLE_PORTABLE  -- the twin member (the header's plain C++ definition)
// Cases: (1) exact data (dyadic coefficients, small-integer samples and histories: every step exact in any precision) for 1, 4, 8
// and 9 samples, in place, and with pfDest one lane above pfSrc (all four lanes are read before any is written); reference below,
// bit for bit, plus guard words and the history write-back. (2) Kernel, inexact samples / histories / gains on dyadic coefficients:
// golden bits from an independent numpy float32 computation (one rounding per SSE operation, the kernel's order). (3) Gather, inexact
// coefficients isolated through one sample of zero input: c2 = 1 - (a1 + a2) appears as the written-back h0, c6 = -(b0 + b1 + b2)
// as the output. Retail is measured under x87 PC_24 and PC_53 (_controlfp): PC_24 = single-precision model with retail's per-lane
// association, PC_53 = the exact sum rounded once. The twin is held to the PC_24 model where float is single precision.
// The coefficients are written with memcpy into the 28-byte CFloatFilterParams (m_fGain, m_fA[3], m_fB[3]; checked below) so the
// twin's member access goes through the real class and its friend. ApplyFilterOrder2_quad does not use `this`; the test calls it
// on dummy storage (constructing a CReverbFnsSSE would need its virtual members, which the portable test does not have).
#include <stdio.h>
#include <string.h>
#include <stddef.h>
#include <math.h>
#if !defined(_MSC_VER)
#define FABLE_RETAIL_PASS23_HOST_TEST_ABI   // CReverbFns.h: the host operator new(size_t) spelling
#endif
#define FABLE_RETAIL_PASS22_FLOATREVERB_CREATE
#define FABLE_RETAIL_CREVERBFNSSSE_APPLYFILTERORDER2_QUAD
#include "engine/CReverbFnsSSE.h"
#include "engine/CFloatFilterParams.h"
#include "engine/ReverbFnsSSEFilterOrder2Quad.h"
// Mode label (the content of fable_test_mode.h, inlined so the file also builds outside rebuild/tests/portable; in the tree it may include it).
#if defined(FABLE_DUAL_PATH_TEST) && !defined(FABLE_PORTABLE)
#define FABLE_TEST_DUAL 1
static const char* const kFableTestMode = "asm+portable";
#elif defined(FABLE_PORTABLE)
static const char* const kFableTestMode = "portable";
#else
static const char* const kFableTestMode = "asm";
#endif
#if !defined(FABLE_PORTABLE)
#pragma comment(lib, "CODAApi.lib")
#endif
#if defined(_MSC_VER) && defined(_M_IX86)
#include <float.h>
#define TEST_X87 1
#endif

typedef char TestFilterParamsSize[(sizeof(CFloatFilterParams) == 7 * sizeof(float)) ? 1 : -1];
static const unsigned int kLane = 128;
static int g_bad = 0;

static float F(unsigned int u) { float f; memcpy(&f, &u, 4); return f; }
static unsigned int U(float f) { unsigned int u; memcpy(&u, &f, 4); return u; }
static void Expect(const char* what, int i, float got, unsigned int want)
{
    if (U(got) != want) { if (g_bad < 12) printf("  %s[%d]: %08X, expected %08X\n", what, i, U(got), want); ++g_bad; }
}

struct Filters {
    CFloatFilterParams p[4];
    CFloatFilterParams* pp[4];
    float hist[4][2];
    float* ph[4];
};
static void SetFilter(Filters* f, int l, float gain, float a1, float a2, float b0, float b1, float b2, float h0, float h1)
{
    const float v[7] = { gain, 1.0f, a1, a2, b0, b1, b2 };   // m_fA[0] (a0) is not read
    memcpy(&f->p[l], v, sizeof v);
    f->pp[l] = &f->p[l];
    f->hist[l][0] = h0; f->hist[l][1] = h1; f->ph[l] = f->hist[l];
}

static void* DummyStorage() { static double d[2]; return d; }
static void CallMember(float* dst, const float* src, Filters* f, unsigned int n)
{
    ((CReverbFnsSSE*)DummyStorage())->ApplyFilterOrder2_quad(dst, src, f->ph, f->pp, n);
}

// ---- (1) exact data ----
static const float kA1[4] = { 0.5f, 0.25f, -0.5f, -0.25f }, kA2[4] = { -0.25f, 0.5f, 0.25f, -0.5f };
static const float kB0[4] = { 1.0f, 2.0f, 0.5f, -1.0f }, kB1[4] = { -0.5f, 0.25f, 1.0f, 0.5f }, kB2[4] = { 0.25f, -0.25f, 0.5f, 2.0f };
static const float kG[4] = { 0.5f, 1.0f, 2.0f, 0.25f }, kH0[4] = { 1.0f, -3.0f, 2.5f, 4.0f }, kH1[4] = { -2.0f, 1.5f, 3.0f, -0.5f };
static void ExactFilters(Filters* f) { for (int l = 0; l < 4; ++l) SetFilter(f, l, kG[l], kA1[l], kA2[l], kB0[l], kB1[l], kB2[l], kH0[l], kH1[l]); }

static int g_inexact = 0;
// v is a float value iff its 53-bit significand fits in 24 bits (pure double arithmetic: exact on x87 PC_53 and on SSE)
static double Ex(double v) { int e; double t = ldexp(frexp(v, &e), 24); if (t != floor(t) || e > 128 || e < -125) ++g_inexact; return v; }
// Reference: per sample, read the four lanes, then write the four lanes (as retail's movss loads / stores).
static void Reference(float* buf, int dstOff, int srcOff, unsigned int n, double h[4][2])
{
    for (unsigned int s = 0; s < n; ++s) {
        double x[4], y[4];
        for (int l = 0; l < 4; ++l) x[l] = buf[srcOff + l * kLane + s];
        for (int l = 0; l < 4; ++l) {
            double c2 = Ex(1.0 - Ex(kA1[l] + (double)kA2[l])), c6 = Ex(-Ex(Ex(kB0[l] + (double)kB1[l]) + kB2[l]));
            double fb = Ex(Ex(kA2[l] * h[l][1]) + Ex(kA1[l] * h[l][0])), ff = Ex(Ex(kB2[l] * h[l][1]) + Ex(kB1[l] * h[l][0]));
            double w = Ex(Ex(x[l] + c2) + fb);
            y[l] = Ex(Ex(Ex(Ex(kB0[l] * w) + ff) + c6) * kG[l]);
            h[l][1] = h[l][0]; h[l][0] = w;
        }
        for (int l = 0; l < 4; ++l) buf[dstOff + l * kLane + s] = (float)y[l];
    }
}

static const int kBuf = 10 * 128 + 16;
static float g_got[kBuf], g_want[kBuf];
static void Fill(float* b) { for (int k = 0; k < kBuf; ++k) b[k] = (float)((int)((k * 37) % 11) - 5); }

static void ExactCase(const char* name, unsigned int n, int dstOff, int srcOff)
{
    Filters f; ExactFilters(&f);
    double h[4][2]; for (int l = 0; l < 4; ++l) { h[l][0] = kH0[l]; h[l][1] = kH1[l]; }
    Fill(g_got); Fill(g_want);
    Reference(g_want, dstOff, srcOff, n, h);
    CallMember(g_got + dstOff, g_got + srcOff, &f, n);
    char what[64];
    for (int k = 0; k < kBuf; ++k) { sprintf(what, "%s buf", name); Expect(what, k, g_got[k], U(g_want[k])); }
    for (int l = 0; l < 4; ++l) {
        sprintf(what, "%s h0", name); Expect(what, l, f.hist[l][0], U((float)h[l][0]));
        sprintf(what, "%s h1", name); Expect(what, l, f.hist[l][1], U((float)h[l][1]));
    }
#if defined(FABLE_TEST_DUAL)
    {   // the twin kernel on the block the member gathers, same inputs, bit for bit against the member's results
        float blk[40], tbuf[kBuf];
        for (int l = 0; l < 4; ++l) {
            blk[l] = kA1[l]; blk[4 + l] = kA2[l]; blk[8 + l] = 1.0f - (kA1[l] + kA2[l]); blk[12 + l] = kB0[l]; blk[16 + l] = kB1[l];
            blk[20 + l] = kB2[l]; blk[24 + l] = -(kB0[l] + kB1[l] + kB2[l]); blk[28 + l] = kH0[l]; blk[32 + l] = kH1[l]; blk[36 + l] = kG[l];
        }
        Fill(tbuf);
        fable_portable::ApplyFilterOrder2QuadKernel(tbuf + dstOff, tbuf + srcOff, n, blk);
        for (int k = 0; k < kBuf; ++k) { sprintf(what, "%s twin buf", name); Expect(what, k, tbuf[k], U(g_got[k])); }
        for (int l = 0; l < 4; ++l) {
            sprintf(what, "%s twin h0", name); Expect(what, l, blk[28 + l], U(f.hist[l][0]));
            sprintf(what, "%s twin h1", name); Expect(what, l, blk[32 + l], U(f.hist[l][1]));
        }
    }
#endif
}

// ---- (2) kernel, inexact data on dyadic coefficients (numpy float32 goldens) ----
static const unsigned int kInexGain[4] = { 0x3EF5C256u, 0x3F7FBDBEu, 0x3F923042u, 0x3EAE2A0Eu };
static const unsigned int kInexH0[4] = { 0xBFB44308u, 0x3FDB3E7Au, 0xBFDBF1D6u, 0xBFBD8E45u };
static const unsigned int kInexH1[4] = { 0x3FE58B4Fu, 0x3EF99E1Bu, 0xBF0626ABu, 0x3D3A9D37u };
static const unsigned int kInexSrc[32] = { 0x3EFA2074u, 0xBF2C9016u, 0xBF8B0530u, 0x3F5D36E3u, 0x3F02D641u, 0x3D182764u, 0x3F7340EBu, 0x3E16C25Du, 0x3FB8ABBCu, 0xBF62EFCEu, 0x3E250F47u, 0xBD493840u, 0xBEE15EADu, 0x3E8CB0BDu, 0xBF4B49E5u, 0x3F681777u, 0x3F8D0E5Cu, 0xBF8E8E69u, 0xBDCA4D5Eu, 0xBF2B2719u, 0xBFA01544u, 0x3F980AE9u, 0xBE573297u, 0xBF87495Bu, 0x3F05246Du, 0xBF64B2B6u, 0x3F9A2648u, 0xBF593AEAu, 0xBFB34CA0u, 0xBF65CF38u, 0xBEECEE67u, 0xBDBF073Bu };
static const unsigned int kInexOut[32] = { 0x3E701EF0u, 0xBEA5A8E4u, 0xBF057580u, 0x3ED45D76u, 0x3E7B34BBu, 0x3C921130u, 0x3EE985CBu, 0x3D90BA74u, 0x404217B3u, 0xBE5BB3FCu, 0x3F99FD99u, 0x3EB6C6A5u, 0xBE770119u, 0x3F11CD72u, 0xBFB170A0u, 0x3FBDC72Du, 0xC0325E5Fu, 0xBF4B67B2u, 0xBF7FA21Au, 0xBF547731u, 0xBFAF35BAu, 0xBF27A818u, 0x3F01F73Cu, 0xBF153445u, 0xBFCF8DECu, 0xBFB2556Fu, 0x3F738828u, 0x3EB061CAu, 0x3F16F77Bu, 0xBF547183u, 0xBF84902Cu, 0x3C843098u };
static const unsigned int kInexH0Final[4] = { 0x3FBF8296u, 0x3FEF7638u, 0x3E6299CEu, 0x3F641EC1u };
static const unsigned int kInexH1Final[4] = { 0x3FF50A08u, 0x3E880C3Cu, 0x3F294B3Fu, 0x3F9FBDC4u };

static void InexactKernelCase()
{
    Filters f;
    for (int l = 0; l < 4; ++l) SetFilter(&f, l, F(kInexGain[l]), kA1[l], kA2[l], kB0[l], kB1[l], kB2[l], F(kInexH0[l]), F(kInexH1[l]));
    static float src[4 * 128], dst[4 * 128];
    memset(src, 0, sizeof src); memset(dst, 0, sizeof dst);
    for (int l = 0; l < 4; ++l) for (int s = 0; s < 8; ++s) src[l * kLane + s] = F(kInexSrc[l * 8 + s]);
    CallMember(dst, src, &f, 8);
    for (int l = 0; l < 4; ++l) {
        for (int s = 0; s < 8; ++s) Expect("inexact out", l * 8 + s, dst[l * kLane + s], kInexOut[l * 8 + s]);
        Expect("inexact h0", l, f.hist[l][0], kInexH0Final[l]);
        Expect("inexact h1", l, f.hist[l][1], kInexH1Final[l]);
    }
}

// ---- (3) gather, inexact coefficients ----
// c6 triple (same in every lane): single precision gives a different sum for each association retail uses; the exact sum
// rounded once (PC_53) is the lane-2 value. c2 pair: single 0xBED815B0, rounded once 0xBED815B2.
static const unsigned int kB[3] = { 0xBEC7AEF9u, 0xBFCE7CCDu, 0x3FEF8729u };
static const unsigned int kC6Single[4] = { 0x3E070B18u, 0x3E070B18u, 0x3E070B12u, 0x3E070B10u };
static const unsigned int kC6Once = 0x3E070B12u;
static const unsigned int kA[2] = { 0x3F1F02A0u, 0x3F4D0839u };
static const unsigned int kC2Single = 0xBED815B0u, kC2Once = 0xBED815B2u;

static void GatherRun(float c6out[4], float c2out[4])
{
    static float src[4 * 128], dst[4 * 128];
    Filters f;
    memset(src, 0, sizeof src); memset(dst, 0, sizeof dst);
    for (int l = 0; l < 4; ++l) SetFilter(&f, l, 1.0f, 0.75f, 0.25f, F(kB[0]), F(kB[1]), F(kB[2]), 0.0f, 0.0f);   // c2 = 0: y = c6
    CallMember(dst, src, &f, 1);
    for (int l = 0; l < 4; ++l) c6out[l] = dst[l * kLane];
    for (int l = 0; l < 4; ++l) SetFilter(&f, l, 1.0f, F(kA[0]), F(kA[1]), 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);       // h0 after one sample = c2
    CallMember(dst, src, &f, 1);
    for (int l = 0; l < 4; ++l) c2out[l] = f.hist[l][0];
}

static void GatherCase(int twinSinglePrecision)
{
    float c6[4], c2[4];
#if defined(TEST_X87) && !defined(FABLE_PORTABLE)
    unsigned int saved = _controlfp(0, 0);
    _controlfp(_PC_24, _MCW_PC); GatherRun(c6, c2); _controlfp(saved, _MCW_PC);
    for (int l = 0; l < 4; ++l) { Expect("retail PC_24 c6", l, c6[l], kC6Single[l]); Expect("retail PC_24 c2", l, c2[l], kC2Single); }
    _controlfp(_PC_53, _MCW_PC); GatherRun(c6, c2); _controlfp(saved, _MCW_PC);
    for (int l = 0; l < 4; ++l) { Expect("retail PC_53 c6", l, c6[l], kC6Once); Expect("retail PC_53 c2", l, c2[l], kC2Once); }
    (void)twinSinglePrecision;
#else
    if (!twinSinglePrecision) return;
    GatherRun(c6, c2);
    for (int l = 0; l < 4; ++l) { Expect("twin c6", l, c6[l], kC6Single[l]); Expect("twin c2", l, c2[l], kC2Single); }
#endif
}

#if defined(TEST_X87) || !defined(__FLT_EVAL_METHOD__) || __FLT_EVAL_METHOD__ != 0
static const int kTwinSinglePrecision = 0;   // x87 (VC7.1, g++ -m32 -mfpmath=387): the twin's float expressions may keep extra precision
#else
static const int kTwinSinglePrecision = 1;
#endif

int main()
{
    ExactCase("n1", 1, 640, 0);
    ExactCase("n4", 4, 640, 0);
    ExactCase("n8", 8, 640, 0);
    ExactCase("n9", 9, 640, 0);
    ExactCase("inplace8", 8, 0, 0);
    ExactCase("dst_lane_up8", 8, 128, 0);
    ExactCase("dst_lane_down8", 8, 0, 128);
    if (g_inexact) { printf("  exact reference left single precision %d times\n", g_inexact); ++g_bad; }
#if defined(FABLE_PORTABLE)
    if (kTwinSinglePrecision) InexactKernelCase();
#else
    InexactKernelCase();
#endif
    GatherCase(kTwinSinglePrecision);
    printf("test_ReverbFnsSSEFilterOrder2Quad %s: %s\n", kFableTestMode, g_bad ? "FAIL" : "PASS");
    return g_bad ? 1 : 0;
}
