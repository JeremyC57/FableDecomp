// Behaviour test for 0x00C52A80 CEngineDebug::CPUAheadCounterCallback: time-stamp counter read, monotonic and advancing.
#include <cstdio>
#ifdef _MSC_VER
typedef unsigned __int64 uint64;
#else
typedef unsigned long long uint64;
#endif
#ifdef _MSC_VER
static uint64 __cdecl CPUAheadCounterCallback()
{
    uint64 counter;
    __asm {
        rdtsc
        mov dword ptr counter, eax
        mov dword ptr counter+4, edx
    }
    return counter;
}
#else
static uint64 CPUAheadCounterCallback() { unsigned int lo, hi; __asm__ __volatile__("rdtsc" : "=a"(lo), "=d"(hi)); return ((uint64)hi << 32) | lo; }
#endif
int main()
{
    uint64 a = CPUAheadCounterCallback();
    unsigned long spin = 0;
    for (int i = 0; i < 100000; ++i) spin += (unsigned long)i;
    uint64 b = CPUAheadCounterCallback();
    if (a == 0 || b <= a || spin == 0) { std::printf("00c52a80_TEST FAIL\n"); return 1; }
    std::printf("00c52a80_TEST PASS\n");
    return 0;
}
