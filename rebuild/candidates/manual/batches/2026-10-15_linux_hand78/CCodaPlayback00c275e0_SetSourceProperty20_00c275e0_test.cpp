#include <cstdio>
static int g_locks = 0, g_unlocks = 0, g_calls = 0, g_lockedDuringCall = 0;
static void* g_source = 0; static float g_value = 0.0f;
struct CProfileLock {
    void* m_pSection; long m_entryTick;
    CProfileLock(void* section) : m_pSection(section), m_entryTick(0) { ++g_locks; }
    ~CProfileLock() { ++g_unlocks; }
};
static unsigned char g_CodaProfileSection[8];
static void CodaSource_SetProperty20(void* source, float value) { ++g_calls; g_source = source; g_value = value; g_lockedDuringCall = g_locks - g_unlocks; }
struct CCodaPlayback_00c275e0 {
    long  m_reserved00;                            // +0x00
    long  m_reserved04;                            // +0x04
    long  m_reserved08;                            // +0x08
    void* m_pSource;                               // +0x0c (the CODA source)
    void SetSourceProperty20(float value)
    {
        CProfileLock lock(g_CodaProfileSection);
        CodaSource_SetProperty20(m_pSource, value);
    }
};
int main()
{
    int source = 0;
    CCodaPlayback_00c275e0 p; p.m_pSource = &source;
    p.SetSourceProperty20(0.5f);
    if (g_calls != 1 || g_source != &source || g_value != 0.5f || g_lockedDuringCall != 1 || g_locks != 1 || g_unlocks != 1) { std::printf("00c275e0_TEST FAIL\n"); return 1; }
    std::printf("00c275e0_TEST PASS\n");
    return 0;
}
