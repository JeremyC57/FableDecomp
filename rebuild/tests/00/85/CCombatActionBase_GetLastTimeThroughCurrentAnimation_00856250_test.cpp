// Behaviour test for CCombatActionBase::GetLastTimeThroughCurrentAnimation (00856250): LastRunningTimeSeconds less the start delay in seconds, clamped at zero.
#include <cstdio>
long g_framesPerSecond = 10;

class CCombatActionBase {
public:
    CCombatActionBase(long delay, float t) : Delay(delay), RunningTimeSeconds(t), LastRunningTimeSeconds(t) { }
    virtual ~CCombatActionBase() { }
    virtual long GetAnimDelayTime() const { return Delay; }
    float GetLastTimeThroughCurrentAnimation() const
    {
        long framesPerSecond = g_framesPerSecond;
        float t = LastRunningTimeSeconds - (float)GetAnimDelayTime() / framesPerSecond;
        if (t < 0.0f)
            t = 0.0f;
        return t;
    }
    long  Delay;
    float RunningTimeSeconds;
    float LastRunningTimeSeconds;
};

int main()
{
    CCombatActionBase a(5, 2.0f);                   // 2.0 - 5/10 = 1.5
    if (a.GetLastTimeThroughCurrentAnimation() != 1.5f) { std::printf("00856250_TEST FAIL\n"); return 1; }
    CCombatActionBase b(30, 2.0f);                  // 2.0 - 3.0 < 0 -> 0
    if (b.GetLastTimeThroughCurrentAnimation() != 0.0f) { std::printf("00856250_TEST FAIL\n"); return 1; }
    g_framesPerSecond = 4;
    CCombatActionBase c(2, 1.0f);                   // 1.0 - 0.5
    if (c.GetLastTimeThroughCurrentAnimation() != 0.5f) { std::printf("00856250_TEST FAIL\n"); return 1; }
    std::printf("00856250_TEST PASS\n");
    return 0;
}