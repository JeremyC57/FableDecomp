#if !defined(_MSC_VER) && !defined(FABLE_PORTABLE)
#define FABLE_PORTABLE   // a non-MSVC build takes the helpers' plain C++ twins
#endif
#include "libpk/MixKernelsSIMD.h"
#include <cstdio>
// behaviour test of ConvertInt128BlockToFloat (0x00C6B3A0): 128 integers to floats on the SSE2, SSE/MMX or x87 path
#ifdef _MSC_VER
#define ALIGN16 __declspec(align(16))
typedef long long32;
#else
#define ALIGN16 __attribute__((aligned(16)))
typedef int long32;
#endif
bool g_cpuDetected = false;
bool g_cpuHasSSE = false;
bool g_cpuHasSSE2 = false;
long g_detectCalls = 0;
void DetectCpu() { ++g_detectCalls; g_cpuDetected = true; }
#ifdef _MSC_VER
#pragma optimize("y", off)
void __cdecl ConvertInt128BlockToFloat(float* dst, const long* src)
{
    if (!g_cpuDetected)
        DetectCpu();
    if (g_cpuHasSSE2) {
        LIBPK_SSE2_CONVERT_INT128_TO_FLOAT(src, dst);
    } else if (g_cpuHasSSE) {
        LIBPK_SSE_CONVERT_INT128_TO_FLOAT(src, dst);
    } else {
        for (long i = 0; i < 128; ++i)
            dst[i] = (float)src[i];
    }
}
#else
void ConvertInt128BlockToFloat(float* dst, const long32* src)
{
    if (!g_cpuDetected)
        DetectCpu();
    for (long i = 0; i < 128; ++i)
        dst[i] = (float)src[i];
}
#endif
static bool Run()
{
    ALIGN16 long32 src[128]; ALIGN16 float dst[129];
    for (int i = 0; i < 128; ++i) { src[i] = i * 1000 - 64000; dst[i] = -1.0f; }
    dst[128] = 7.0f;
    ConvertInt128BlockToFloat(dst, src);
    bool ok = dst[128] == 7.0f;
    for (int i = 0; i < 128; ++i) ok = ok && dst[i] == (float)(i * 1000 - 64000);
    return ok;
}
int main()
{
    bool ok = Run() && g_detectCalls == 1;                   // x87 path, detection ran once
    ok = ok && Run() && g_detectCalls == 1;                  // already detected
    g_cpuHasSSE = true;  ok = ok && Run();                   // SSE/MMX path
    g_cpuHasSSE2 = true; ok = ok && Run();                   // SSE2 path
    std::printf(ok ? "00c6b3a0_TEST PASS\n" : "00c6b3a0_TEST FAIL\n");
    return ok ? 0 : 1;
}