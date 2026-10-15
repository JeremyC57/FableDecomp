#include <cstdio>
#ifdef _MSC_VER
typedef __int64 Int64;
#else
typedef long long Int64;
#endif
// Behaviour test for 0062f900 (CStopWatch on the PDB layout).
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
    float GetElapsedSeconds() const;
};
float CStopWatch::GetElapsedSeconds() const
{
    Int64 ticks = m_nPrevElapsedTicks;
    if (m_bIsRunning)
        ticks += GetTicks() - m_nStartTick;
    return ticks * m_fTimerPeriod;
}
#define FAIL do { std::printf("0062f900_TEST FAIL\n"); return 1; } while (0)
int main()
{
    CStopWatch w; w.m_fTimerPeriod = 0.5f; w.m_nStartTick = 1000; w.m_nPrevElapsedTicks = 40; w.m_bIsRunning = false;
    g_counterValue = 1100;
    if (w.GetElapsedSeconds() != 20.0f) FAIL;
    w.m_bIsRunning = true;
    if (w.GetElapsedSeconds() != 70.0f || w.m_nPrevElapsedTicks != 40) FAIL;
    std::printf("0062f900_TEST PASS\n");
    return 0;
}
