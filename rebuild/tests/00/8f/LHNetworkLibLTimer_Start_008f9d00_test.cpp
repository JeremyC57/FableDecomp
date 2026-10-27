// Behaviour test for 0x008F9D00 LHNetworkLib::LTimer::Start(): with a near-zero speed the timer time is rebased to
// MSeconds() (the stopped interval adds ~nothing), the clock is re-read, and the saved speed is restored.
#include <cstdio>

static unsigned long g_tick = 0;
static unsigned long GetTicks() { return g_tick; }

class Timer {
public:
    unsigned long SystemClockAtLastAdjust, TimerTimeAtLastAdjust;
    float SpeedUpFactor, SaveSpeedUpFactor;
    unsigned long MSeconds() { return (unsigned long)((float)(GetTicks() - SystemClockAtLastAdjust) * SpeedUpFactor + (float)TimerTimeAtLastAdjust); }
    void Start()
    {
        SpeedUpFactor = 0.00001f;
        float saved = SaveSpeedUpFactor;
        TimerTimeAtLastAdjust = MSeconds();
        SystemClockAtLastAdjust = GetTicks();
        SpeedUpFactor = saved;
    }
};

int main()
{
    Timer t;
    t.SystemClockAtLastAdjust = 1000; t.TimerTimeAtLastAdjust = 500; t.SpeedUpFactor = 0.0f; t.SaveSpeedUpFactor = 2.0f;
    g_tick = 5000;                                   // 4000 ms stopped
    t.Start();
    if (t.SpeedUpFactor != 2.0f || t.SystemClockAtLastAdjust != 5000 || t.TimerTimeAtLastAdjust != 500) { std::printf("008f9d00_TEST FAIL\n"); return 1; }
    g_tick = 5100;
    if (t.MSeconds() != 700) { std::printf("008f9d00_TEST FAIL\n"); return 1; }
    std::printf("008f9d00_TEST PASS\n");
    return 0;
}