// test_MixKernelsSIMD.cpp -- dual-path test of libpk/MixKernelsSIMD.h: every libpk MMX / SSE / SSE2 kernel macro on
// pseudo-random 16-bit samples, gains and ramp steps (including saturating ramps and the wrapping 32-bit accumulators), 128-entry
// int / float blocks (ties, negatives, out-of-range and NaN floats) and the copy / zero blocks, against plain reference loops
// and (dual) the fable_portable twins. Every kernel needs SSE2 (the lane runs on an SSE2 host).
#include <stdio.h>
#include <string.h>
#include "libpk/MixKernelsSIMD.h"
#include "fable_test_mode.h"
typedef int i32;
static unsigned int g_seed = 12345u;
static unsigned int Rand() { g_seed = g_seed * 1103515245u + 12345u; return g_seed >> 8; }
static short RandShort() { return (short)(Rand() & 0xFFFF); }
// kernel wrappers: each macro on the wrapper's own parameters and aligned locals, as the libpk rows use them
static void MmxAcc(unsigned long count, const short* src, i32* dst, const short* g)
{ FABLE_TEST_ALIGN16 short gains[4]; memcpy(gains, g, 8);
    LIBPK_MMX_SCALE_ACCUMULATE16(count, src, dst, gains);
}
static void MmxRamp(unsigned long count, const short* src, i32* dst, const short* g, const short* s)
{ FABLE_TEST_ALIGN16 short gains[4]; FABLE_TEST_ALIGN16 short increments[4]; memcpy(gains, g, 8); memcpy(increments, s, 8);
      LIBPK_MMX_SCALE_ACCUMULATE16_RAMP(count, src, dst, gains, increments);
  }
static void Sse2Acc(unsigned long count, const short* src, i32* dst, const short* g)
{ FABLE_TEST_ALIGN16 short gains[8]; memcpy(gains, g, 16);
    LIBPK_SSE2_SCALE_ACCUMULATE16(count, src, dst, gains);
}
static void Sse2Ramp(unsigned long count, const short* src, i32* dst, const short* g, const short* s)
{ FABLE_TEST_ALIGN16 short gains[8]; FABLE_TEST_ALIGN16 short increments[8]; memcpy(gains, g, 16); memcpy(increments, s, 16);
      LIBPK_SSE2_SCALE_ACCUMULATE16_RAMP(count, src, dst, gains, increments);
  }
static void Sse2AccU(unsigned long count, const short* src, i32* dst, const short* g)
{ FABLE_TEST_ALIGN16 short gains[8]; memcpy(gains, g, 16);
    LIBPK_SSE2_SCALE_ACCUMULATE16_UNALIGNED(count, src, dst, gains);
}
static void Sse2RampU(unsigned long count, const short* src, i32* dst, const short* g, const short* s)
{ FABLE_TEST_ALIGN16 short gains[8]; FABLE_TEST_ALIGN16 short increments[8]; memcpy(gains, g, 16); memcpy(increments, s, 16);
      LIBPK_SSE2_SCALE_ACCUMULATE16_RAMP_UNALIGNED(count, src, dst, gains, increments);
  }
static void AddBlock(const i32* src, i32* dst) {
    LIBPK_SSE2_ADD_INT128_BLOCK(src, dst);
}
static void AccFloat(const float* src, i32* dst) {
    LIBPK_SSE2_ACCUMULATE_FLOAT128_BLOCK(src, dst);
}
static void I2F2(const i32* src, float* dst) {
    LIBPK_SSE2_CONVERT_INT128_TO_FLOAT(src, dst);
}
static void I2F1(const i32* src, float* dst) {
    LIBPK_SSE_CONVERT_INT128_TO_FLOAT(src, dst);
}
static void I2FIP2(void* block) {
    LIBPK_SSE2_CONVERT_INT128_TO_FLOAT_IN_PLACE(block);
}
static void I2FIP1(void* block) {
    LIBPK_SSE_CONVERT_INT128_TO_FLOAT_IN_PLACE(block);
}
static void F2IIP2(float* block) { unsigned long csr;
    LIBPK_SSE2_CONVERT_FLOAT128_TO_INT_IN_PLACE(csr, block);
}
static void F2IIP1(float* block) { unsigned long csr;
    LIBPK_SSE_CONVERT_FLOAT128_TO_INT_IN_PLACE(csr, block);
}
static void Copy64(const void* source, void* history) {
    LIBPK_SSE2_COPY64(source, history);
}
static void Copy128(const void* src, void* dst) {
    LIBPK_SSE2_COPY128(src, dst);
}
static void MmxCopy128(const void* src, void* dst) {
    LIBPK_MMX_COPY128(src, dst);
}
static void Zero64(void* history) {
    LIBPK_SSE_ZERO64(history);
}
static void MmxZero64(void* history) {
    LIBPK_MMX_ZERO64(history);
}
static void Zero128(void* tail) {
    LIBPK_SSE_ZERO128(tail);
}
static void MmxZero128(void* tail) {
    LIBPK_MMX_ZERO128(tail);
}
// references
static int Sat(int v) { return v > 32767 ? 32767 : (v < -32768 ? -32768 : v); }
static int High(int a, int b) { int p = a * b; return (p - (p & 0xFFFF)) / 65536; }
static void RefAcc(unsigned long groups, int lanes, const short* src, i32* dst, const short* g0, const short* step)
{
    int g[8];
    for (int l = 0; l < lanes; ++l) g[l] = g0[l];
    for (unsigned long n = 0; n < groups; ++n)
        for (int l = 0; l < lanes; ++l) {
            unsigned int d = (unsigned int)dst[n * lanes + l] + (unsigned int)(High(src[n * lanes + l], g[l]) * 2);
            memcpy(&dst[n * lanes + l], &d, 4);
            if (l == lanes - 1 && step) for (int k = 0; k < lanes; ++k) g[k] = Sat(g[k] + step[k]);
        }
}
static i32 RefRound(float f)
{
    if (!(f >= -2147483648.0f && f < 2147483648.0f)) return (i32)0x80000000u;
    double r = (double)f; double fl = (double)(i32)r; if (fl > r) fl -= 1.0;
    double fr = r - fl; i32 v = (i32)fl;
    if (fr > 0.5 || (fr == 0.5 && (v & 1))) ++v;
    return v;
}
static int g_bad = 0;
static void Expect(const void* a, const void* b, unsigned int bytes, const char* what)
{
    if (memcmp(a, b, bytes)) { printf("  %s differs\n", what); ++g_bad; }
}
int main()
{
    FABLE_TEST_ALIGN16 short src[8 * 40 + 8];
    FABLE_TEST_ALIGN16 i32 dst[8 * 40], ref[8 * 40];
    short gains[8], steps[8];
    for (int round = 0; round < 6; ++round) {
        for (int i = 0; i < 8 * 40 + 8; ++i) src[i] = RandShort();
        for (int i = 0; i < 8; ++i) { gains[i] = round == 0 ? (short)32767 : RandShort(); steps[i] = (short)(round < 3 ? (Rand() & 0x7FF) : RandShort()); }
        for (unsigned long count = 1; count <= 20; count += 19) {
            for (int i = 0; i < 8 * 40; ++i) dst[i] = ref[i] = (i32)(Rand() * 977u);
            MmxAcc(count, src, dst, gains); RefAcc(count * 2, 4, src, ref, gains, 0); Expect(dst, ref, sizeof dst, "MMX_SCALE_ACCUMULATE16");
            MmxRamp(count, src, dst, gains, steps); RefAcc(count * 2, 4, src, ref, gains, steps); Expect(dst, ref, sizeof dst, "MMX_SCALE_ACCUMULATE16_RAMP");
            Sse2Acc(count, src, dst, gains); RefAcc(count, 8, src, ref, gains, 0); Expect(dst, ref, sizeof dst, "SSE2_SCALE_ACCUMULATE16");
            Sse2Ramp(count, src, dst, gains, steps); RefAcc(count, 8, src, ref, gains, steps); Expect(dst, ref, sizeof dst, "SSE2_SCALE_ACCUMULATE16_RAMP");
            Sse2AccU(count, src + 1, dst, gains); RefAcc(count, 8, src + 1, ref, gains, 0); Expect(dst, ref, sizeof dst, "SSE2_SCALE_ACCUMULATE16_UNALIGNED");
            Sse2RampU(count, src + 3, dst, gains, steps); RefAcc(count, 8, src + 3, ref, gains, steps); Expect(dst, ref, sizeof dst, "SSE2_SCALE_ACCUMULATE16_RAMP_UNALIGNED");
#ifdef FABLE_TEST_DUAL
            { FABLE_TEST_ALIGN16 i32 a[8 * 40], b[8 * 40]; for (int i = 0; i < 8 * 40; ++i) a[i] = b[i] = (i32)(i * 40503u);
              MmxRamp(count, src, a, gains, steps); fable_portable::ScaleAccumulate16(count, src, b, gains, steps, 4, 2); Expect(a, b, sizeof a, "twin MMX ramp");
              Sse2RampU(count, src + 3, a, gains, steps); fable_portable::ScaleAccumulate16(count, src + 3, b, gains, steps, 8, 1); Expect(a, b, sizeof a, "twin SSE2 ramp");
              MmxAcc(count, src, a, gains); fable_portable::ScaleAccumulate16(count, src, b, gains, 0, 4, 2); Expect(a, b, sizeof a, "twin MMX acc"); }
#endif
        }
    }
    // 128-entry blocks
    FABLE_TEST_ALIGN16 i32 ia[128], ib[128], ic[128];
    FABLE_TEST_ALIGN16 float fa[128], fb[128], fc[128];
    for (int i = 0; i < 128; ++i) {
        ia[i] = (i32)(Rand() * 2654435761u); ib[i] = ic[i] = (i32)(Rand() * 40503u);
        static const float special[] = { 0.5f, 1.5f, 2.5f, -0.5f, -1.5f, -2.5f, 3e9f, -3e9f, 2147483520.0f, -2147483648.0f, 0.49999997f };
        fa[i] = i < 11 ? special[i] : (float)(i32)(Rand() & 0xFFFFF) / 7.0f - 50000.0f;
    }
    { unsigned int nan = 0x7FC00000u; memcpy(&fa[11], &nan, 4); }
    AddBlock(ia, ib); for (int i = 0; i < 128; ++i) { unsigned int d = (unsigned int)ic[i] + (unsigned int)ia[i]; memcpy(&ic[i], &d, 4); } Expect(ib, ic, sizeof ib, "SSE2_ADD_INT128_BLOCK");
    AccFloat(fa, ib); for (int i = 0; i < 128; ++i) { unsigned int d = (unsigned int)ic[i] + (unsigned int)RefRound(fa[i]); memcpy(&ic[i], &d, 4); } Expect(ib, ic, sizeof ib, "SSE2_ACCUMULATE_FLOAT128_BLOCK");
    for (int i = 0; i < 128; ++i) fc[i] = (float)ia[i];
    I2F2(ia, fb); Expect(fb, fc, sizeof fb, "SSE2_CONVERT_INT128_TO_FLOAT");
    I2F1(ia, fb); Expect(fb, fc, sizeof fb, "SSE_CONVERT_INT128_TO_FLOAT");
    memcpy(fb, ia, sizeof fb); I2FIP2(fb); Expect(fb, fc, sizeof fb, "SSE2_CONVERT_INT128_TO_FLOAT_IN_PLACE");
    memcpy(fb, ia, sizeof fb); I2FIP1(fb); Expect(fb, fc, sizeof fb, "SSE_CONVERT_INT128_TO_FLOAT_IN_PLACE");
    for (int i = 0; i < 128; ++i) ic[i] = RefRound(fa[i]);
    memcpy(fb, fa, sizeof fb); F2IIP2(fb); Expect(fb, ic, sizeof fb, "SSE2_CONVERT_FLOAT128_TO_INT_IN_PLACE");
    memcpy(fb, fa, sizeof fb); F2IIP1(fb); Expect(fb, ic, sizeof fb, "SSE_CONVERT_FLOAT128_TO_INT_IN_PLACE");
#ifdef FABLE_TEST_DUAL
    { FABLE_TEST_ALIGN16 float q[128]; memcpy(q, fa, sizeof q); fable_portable::ConvertFloat128ToIntNearest(q); memcpy(fb, fa, sizeof fb); F2IIP2(fb); Expect(fb, q, sizeof q, "twin F2I");
      fable_portable::ConvertInt128ToFloat(ia, q); I2F2(ia, fb); Expect(fb, q, sizeof q, "twin I2F"); }
#endif
    FABLE_TEST_ALIGN16 unsigned char s8[128], d8[128], r8[128];
    for (int i = 0; i < 128; ++i) s8[i] = (unsigned char)Rand();
    memset(d8, 0xAA, 128); memset(r8, 0xAA, 128); memcpy(r8, s8, 64); Copy64(s8, d8); Expect(d8, r8, 128, "SSE2_COPY64");
    memset(d8, 0xAA, 128); Copy128(s8, d8); Expect(d8, s8, 128, "SSE2_COPY128");
    memset(d8, 0xAA, 128); MmxCopy128(s8, d8); Expect(d8, s8, 128, "MMX_COPY128");
    memset(d8, 0xAA, 128); memset(r8, 0xAA, 128); memset(r8, 0, 64); Zero64(d8); Expect(d8, r8, 128, "SSE_ZERO64");
    memset(d8, 0xAA, 128); MmxZero64(d8); Expect(d8, r8, 128, "MMX_ZERO64");
    memset(d8, 0xAA, 128); memset(r8, 0, 128); Zero128(d8); Expect(d8, r8, 128, "SSE_ZERO128");
    memset(d8, 0xAA, 128); MmxZero128(d8); Expect(d8, r8, 128, "MMX_ZERO128");
    // after the MMX kernels (emms) the x87 stack must be usable

    {
        float x = 1.25f, y = (float)(i32)3 * x;
        if (y != 3.75f) { printf("  x87 unusable after emms\n"); ++g_bad; }
    }
    printf("test_MixKernelsSIMD %s: %s\n", kFableTestMode, g_bad ? "FAIL" : "PASS");
    return g_bad ? 1 : 0;
}
