// Behaviour test for 00c3cfe0: CAudioTimer::GetElapsedTicks -- the performance counter now minus the start count stored at +0x190
#include <cstdio>

#ifdef _MSC_VER
typedef __int64 int64;
#else
typedef long long int64;
#endif

static int64 g_counter = 0;
static int QueryPerformanceCounter(int64* count) { *count = g_counter; return 1; }

struct CAudioTimer {
    int64 m_startCount;
    int64 GetElapsedTicks()
    {
        int64 now;
        QueryPerformanceCounter(&now);
        return now - m_startCount;
    }
};

int main()
{
    CAudioTimer t;
    t.m_startCount = 1000;
    g_counter = ((int64)1 << 32) + 1500;
    if (t.GetElapsedTicks() != ((int64)1 << 32) + 500) { std::printf("00c3cfe0_TEST FAIL\n"); return 1; }
    std::printf("00c3cfe0_TEST PASS\n");
    return 0;
}