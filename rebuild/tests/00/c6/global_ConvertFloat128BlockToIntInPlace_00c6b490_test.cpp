// Behaviour test for 0x00C6B490 ConvertFloat128BlockToIntInPlace (libpk mixer): every path rounds the 128 floats of the
// block to nearest 32-bit integers in place; the SIMD paths force round-to-nearest in MXCSR and restore the caller's
// rounding bits afterwards. The SIMD loops are modelled in C (the asm itself is graded byte-exact), the MXCSR as a word.
#include <cstdio>

static bool g_cpuDetected = false, g_cpuHasSSE = false, g_cpuHasSSE2 = false;
static int g_detects = 0;
static unsigned int g_mxcsr = 0x1F80;
static void DetectCpu() { ++g_detects; g_cpuDetected = true; }

static int RoundNearest(float f)                // round half to even (the default x87 / MXCSR mode)
{
    double d = (double)f;
    double fl = (double)(long)d; if (fl > d) fl -= 1.0;
    double frac = d - fl;
    long r = (long)fl;
    if (frac > 0.5 || (frac == 0.5 && (r & 1))) ++r;
    return (int)r;
}
static int FtoL(float f) { return RoundNearest(f); }

static void SimdRound(float* block, int lanes)
{
    unsigned int old = g_mxcsr & 0x6000;
    g_mxcsr &= 0xFFFF9FFF;                      // round to nearest
    for (int i = 0; i < 128; i += lanes)
        for (int j = 0; j < lanes; ++j)
            ((int*)block)[i + j] = RoundNearest(block[i + j]);
    g_mxcsr = (g_mxcsr & 0xFFFF9FFF) | old;     // the caller's rounding bits back
}

static void ConvertFloat128BlockToIntInPlace(float* block)
{
    if (!g_cpuDetected)
        DetectCpu();
    if (g_cpuHasSSE2)
        SimdRound(block, 4);
    else if (g_cpuHasSSE)
        SimdRound(block, 2);
    else
        for (unsigned int i = 0; i < 128; ++i)
            ((int*)block)[i] = FtoL(block[i]);
}

int main()
{
    if (sizeof(int) != sizeof(float)) { std::printf("00c6b490_TEST FAIL\n"); return 1; }
    for (int path = 0; path < 3; ++path) {
        g_cpuHasSSE2 = path == 0; g_cpuHasSSE = path == 1;
        g_mxcsr = 0x1F80 | 0x6000;              // caller runs in truncate mode
        float block[128];
        for (int i = 0; i < 128; ++i) block[i] = (float)(i - 64) * 0.75f + 0.1f;
        ConvertFloat128BlockToIntInPlace(block);
        for (int i = 0; i < 128; ++i) {
            float f = (float)(i - 64) * 0.75f + 0.1f;
            if (((int*)block)[i] != RoundNearest(f)) { std::printf("00c6b490_TEST FAIL\n"); return 1; }
        }
        if (g_mxcsr != (0x1F80 | 0x6000)) { std::printf("00c6b490_TEST FAIL\n"); return 1; }
    }
    if (g_detects != 1 || RoundNearest(2.5f) != 2 || RoundNearest(-1.6f) != -2) { std::printf("00c6b490_TEST FAIL\n"); return 1; }
    std::printf("00c6b490_TEST PASS\n");
    return 0;
}