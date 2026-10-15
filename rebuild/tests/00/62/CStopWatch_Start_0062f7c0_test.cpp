#include <cstdio>
#ifdef _MSC_VER
typedef __int64 Int64;
#else
typedef long long Int64;
#endif
// Behaviour test for 0062f7c0 (CStopWatch on the PDB layout).
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
    void Start();
};
void CStopWatch::Start()
{
    m_nStartTick = GetTicks();
    m_bIsRunning = true;
}
#define FAIL do { std::printf("0062f7c0_TEST FAIL\n"); return 1; } while (0)
int main()
{
    CStopWatch w; w.m_fTimerPeriod = 1.0f; w.m_nStartTick = 5; w.m_nPrevElapsedTicks = 70; w.m_bIsRunning = false;
    g_counterValue = ((Int64)5 * 1000000000); w.Start();
    if (w.m_nStartTick != ((Int64)5 * 1000000000) || !w.m_bIsRunning || w.m_nPrevElapsedTicks != 70) FAIL;
    g_counterAvailable = 0; w.m_bIsRunning = false; w.Start();
    if (w.m_nStartTick != 0 || !w.m_bIsRunning) FAIL;
    std::printf("0062f7c0_TEST PASS\n");
    return 0;
}