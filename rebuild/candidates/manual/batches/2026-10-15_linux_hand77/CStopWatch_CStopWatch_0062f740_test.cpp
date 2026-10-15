#include <cstdio>
#ifdef _MSC_VER
typedef __int64 Int64;
#else
typedef long long Int64;
#endif
// Behaviour test for 0062f740 (CStopWatch on the PDB layout).
// QueryPerformanceCounter / QueryPerformanceFrequency stubs: availability and values are scripted per test.
static int   g_counterAvailable = 1;
static Int64 g_counterValue = 0;
static int   g_frequencyAvailable = 1;
static Int64 g_frequencyValue = 1000;
static int QueryPerformanceCounter(Int64* count) { if (!g_counterAvailable) return 0; *count = g_counterValue; return 1; }
static int QueryPerformanceFrequency(Int64* frequency) { if (!g_frequencyAvailable) return 0; *frequency = g_frequencyValue; return 1; }
class CStopWatch {
public:
    float m_fTimerPeriod;
    Int64 m_nStartTick;
    Int64 m_nPrevElapsedTicks;
    bool  m_bIsRunning;
    static Int64 GetTicks()
    {
        Int64 ticks;
        if (QueryPerformanceCounter(&ticks))
            return ticks;
        return 0;
    }
    CStopWatch();
};
CStopWatch::CStopWatch()
    : m_fTimerPeriod(0), m_nStartTick(0), m_nPrevElapsedTicks(0), m_bIsRunning(false)
{
    Int64 frequency;
    if (QueryPerformanceFrequency(&frequency))
        m_fTimerPeriod = 1.0f / frequency;
    else
        m_fTimerPeriod = 1.0f;
}
#define FAIL do { std::printf("0062f740_TEST FAIL\n"); return 1; } while (0)
int main()
{
    g_frequencyValue = 1000;
    CStopWatch a;
    if (a.m_fTimerPeriod != 1.0f / 1000 || a.m_nStartTick != 0 || a.m_nPrevElapsedTicks != 0 || a.m_bIsRunning) FAIL;
    g_frequencyAvailable = 0;
    CStopWatch b;
    if (b.m_fTimerPeriod != 1.0f || b.m_nStartTick != 0 || b.m_bIsRunning) FAIL;
    std::printf("0062f740_TEST PASS\n");
    return 0;
}
