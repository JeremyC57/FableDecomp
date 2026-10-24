// Behaviour test for GFGetTime (0x009E1BC0): counter path (delta / frequency) and timeGetTime fallback (ms * 0.001).
#include <cstdio>

#ifdef _MSC_VER
typedef __int64 Int64;
#else
typedef long long Int64;
#endif

union LargeInteger { struct { unsigned int LowPart; int HighPart; } u; Int64 QuadPart; };

static Int64 g_fakeCounter = 0;
static int g_counterWorks = 1;
static unsigned int g_fakeTicks = 0;
static int QueryPerformanceCounterFake(LargeInteger* c) { c->QuadPart = g_fakeCounter; return g_counterWorks; }
static int QueryPerformanceFrequencyFake(LargeInteger* f) { f->QuadPart = 1000000; return 1; }
static unsigned int timeGetTimeFake() { return g_fakeTicks; }

Int64 g_TimerStartCount = 0;
static bool g_HasPerformanceCounter;
static LargeInteger g_PerformanceFrequency;
static bool g_TimerInitialised;

double GFGetTime()
{
    if (!g_TimerInitialised)
    {
        LargeInteger probe;
        g_HasPerformanceCounter = QueryPerformanceCounterFake(&probe) == 1;
        QueryPerformanceFrequencyFake(&g_PerformanceFrequency);
        g_TimerInitialised = true;
    }
    if (g_HasPerformanceCounter)
    {
        LargeInteger count;
        QueryPerformanceCounterFake(&count);
        Int64 elapsed = count.QuadPart - g_TimerStartCount;
        return (double)elapsed / (double)g_PerformanceFrequency.QuadPart;
    }
    return (double)((Int64)timeGetTimeFake() - g_TimerStartCount) * 0.001;
}

static bool Near(double a, double b) { double d = a - b; return d < 1e-6 && d > -1e-6; }

int main()
{
    g_TimerStartCount = 500000;
    g_fakeCounter = 2500000;
    if (!Near(GFGetTime(), 2.0)) { std::printf("009e1bc0_TEST FAIL\n"); return 1; }
    if (!g_TimerInitialised || !g_HasPerformanceCounter) { std::printf("009e1bc0_TEST FAIL\n"); return 1; }
    // fallback path: counter unusable at first query
    g_TimerInitialised = false;
    g_counterWorks = 0;
    g_TimerStartCount = 1000;
    g_fakeTicks = 3500;
    if (!Near(GFGetTime(), 2.5)) { std::printf("009e1bc0_TEST FAIL\n"); return 1; }
    if (g_HasPerformanceCounter) { std::printf("009e1bc0_TEST FAIL\n"); return 1; }
    std::printf("009e1bc0_TEST PASS\n");
    return 0;
}