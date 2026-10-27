#include <cstdio>
#include <cstring>
#include <cmath>
static bool g_cpuDetected = false; static int g_detects = 0;
static void DetectCpu() { ++g_detects; g_cpuDetected = true; }
struct CCpuDetectInit { CCpuDetectInit() { if (!g_cpuDetected) DetectCpu(); } };

int main()
{
    { CCpuDetectInit a; }
    if (g_detects != 1) do { std::printf("01224c20_TEST FAIL\n"); return 1; } while (0);
    { CCpuDetectInit b; }                                 // already detected: no second call
    if (g_detects != 1) do { std::printf("01224c20_TEST FAIL\n"); return 1; } while (0);
    std::printf("01224c20_TEST PASS\n");
    return 0;
}