// Behaviour test for 0x00C5B250 CSoftwareVoice::StartSound
#include <cstdio>
static long long g_performanceFrequency = 3579545;
static int g_begins = 0, g_resets = 0, g_schedules = 0;
struct Voice {
    void* m_pBuffer; long long m_clock; void* m_pActiveSound;
    void BeginSound(void* s) { m_pActiveSound = s; ++g_begins; }
    void Reset() { ++g_resets; }
    void ScheduleSound(void*, long long) { ++g_schedules; }
    int StartSound(void* sound, long long startTime)
    {
        if (!m_pActiveSound) { m_pBuffer = 0; BeginSound(sound); Reset(); }
        ScheduleSound(sound, startTime);
        if (m_clock < g_performanceFrequency * 128 / 44100 + startTime) return 2;
        if (m_pActiveSound) ScheduleSound(sound, startTime);
        return 1;
    }
};
static int Fail() { std::printf("00c5b250_TEST FAIL\n"); return 1; }
int main()
{
    int snd = 0; Voice v; v.m_pBuffer = &snd; v.m_clock = 1000; v.m_pActiveSound = 0;
    long long block = g_performanceFrequency * 128 / 44100;         // 10389 ticks
    if (v.StartSound(&snd, 0) != 2 || v.m_pBuffer != 0 || g_begins != 1 || g_resets != 1 || g_schedules != 1) return Fail();
    v.m_clock = block + 50;
    if (v.StartSound(&snd, 50) != 1 || g_begins != 1 || g_schedules != 3) return Fail();   // clock reached: scheduled twice
    v.m_clock = 0x100000000LL;                                       // 64-bit comparison
    if (v.StartSound(&snd, 0x100000000LL - block - 1) != 1) return Fail();
    std::printf("00c5b250_TEST PASS\n");
    return 0;
}