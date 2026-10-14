#include <cstdio>
union _LARGE_INTEGER { struct { unsigned long LowPart; long HighPart; } u; long long QuadPart; };
static int g_qpcOk = 1, g_calls = 0; static int QueryPerformanceCounter(_LARGE_INTEGER* c) { ++g_calls; c->QuadPart = 0x100000005LL; return g_qpcOk; }
static unsigned long timeGetTime() { return 777; } static long long g_start = 0;
static void InitialiseTimerBase() { _LARGE_INTEGER count; if (QueryPerformanceCounter(&count) == 1) { QueryPerformanceCounter(&count); g_start = count.QuadPart; } else { g_start = timeGetTime(); } }
int main() { InitialiseTimerBase(); if (g_start != 0x100000005LL || g_calls != 2) { std::printf("009e1cb0_TEST FAIL\n"); return 1; } g_qpcOk = 0; InitialiseTimerBase(); if (g_start != 777 || g_calls != 3) { std::printf("009e1cb0_TEST FAIL\n"); return 1; } std::printf("009e1cb0_TEST PASS\n"); return 0; }
