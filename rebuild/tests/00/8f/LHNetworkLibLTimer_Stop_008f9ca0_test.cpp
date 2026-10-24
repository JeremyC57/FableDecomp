// Behaviour test for 0x008F9CA0 LTimer::Stop: a running timer saves its factor and is set to factor 0.
#include <cstdio>

class LTimer {
public:
    char          buf[256];
    unsigned long SystemClockAtLastAdjust;
    unsigned long TimerTimeAtLastAdjust;
    float         SpeedUpFactor;
    float         SaveSpeedUpFactor;
    int           m_setCalls;
    float         m_lastSet;
    int Running() { return SpeedUpFactor != 0.0f; }
    float GetSpeedUpFactor() { return Running() ? SpeedUpFactor : SaveSpeedUpFactor; }
    void SetSpeedUpFactor(float factor) { ++m_setCalls; m_lastSet = factor; if (Running()) SpeedUpFactor = factor; else SaveSpeedUpFactor = factor; }
    void Stop();
};

void LTimer::Stop()
{
    if (Running()) {
        SaveSpeedUpFactor = GetSpeedUpFactor();
        SetSpeedUpFactor(0.0f);
    }
}

int main()
{
    int fail = 0;
    LTimer t;
    t.SpeedUpFactor = 1.5f; t.SaveSpeedUpFactor = 9.0f; t.m_setCalls = 0; t.m_lastSet = -1.0f;
    t.Stop();
    if (t.SaveSpeedUpFactor != 1.5f || t.SpeedUpFactor != 0.0f || t.m_setCalls != 1 || t.m_lastSet != 0.0f) fail = 1;
    t.Stop();                                        // already stopped: nothing happens
    if (t.SaveSpeedUpFactor != 1.5f || t.m_setCalls != 1) fail = 1;
    if (fail) { std::printf("008f9ca0_TEST FAIL\n"); return 1; }
    std::printf("008f9ca0_TEST PASS\n");
    return 0;
}