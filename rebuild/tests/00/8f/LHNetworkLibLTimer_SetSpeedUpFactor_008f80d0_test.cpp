// Behaviour test for 0x008F80D0 LTimer::SetSpeedUpFactor: running -> re-base and take the factor; stopped -> save it.
#include <cstdio>

static unsigned long g_tick = 1000;
static unsigned long FakeGetTickCount() { return g_tick; }

class LTimer {
public:
    char          buf[256];
    unsigned long SystemClockAtLastAdjust;
    unsigned long TimerTimeAtLastAdjust;
    float         SpeedUpFactor;
    float         SaveSpeedUpFactor;
    unsigned long MSeconds()
    {
        return (unsigned long)((float)(FakeGetTickCount() - SystemClockAtLastAdjust) * SpeedUpFactor + (float)TimerTimeAtLastAdjust);
    }
    int Running() { return SpeedUpFactor != 0.0f; }
    void SetSpeedUpFactor(float factor);
};

void LTimer::SetSpeedUpFactor(float factor)
{
    if (Running()) {
        TimerTimeAtLastAdjust = MSeconds();
        SystemClockAtLastAdjust = FakeGetTickCount();
        SpeedUpFactor = factor;
    } else {
        SaveSpeedUpFactor = factor;
    }
}

int main()
{
    int fail = 0;
    LTimer t;
    t.SystemClockAtLastAdjust = 1000; t.TimerTimeAtLastAdjust = 50; t.SpeedUpFactor = 2.0f; t.SaveSpeedUpFactor = 7.0f;
    g_tick = 1100;                                   // 100 real ms at x2 -> timer 250
    t.SetSpeedUpFactor(0.5f);
    if (t.TimerTimeAtLastAdjust != 250 || t.SystemClockAtLastAdjust != 1100 || t.SpeedUpFactor != 0.5f || t.SaveSpeedUpFactor != 7.0f) fail = 1;
    t.SpeedUpFactor = 0.0f;                          // stopped: only the saved factor changes
    g_tick = 5000;
    t.SetSpeedUpFactor(3.0f);
    if (t.TimerTimeAtLastAdjust != 250 || t.SystemClockAtLastAdjust != 1100 || t.SpeedUpFactor != 0.0f || t.SaveSpeedUpFactor != 3.0f) fail = 1;
    if (fail) { std::printf("008f80d0_TEST FAIL\n"); return 1; }
    std::printf("008f80d0_TEST PASS\n");
    return 0;
}