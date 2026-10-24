// Behaviour test for CCombatActionBase::GetTimeThroughCurrentAnimation (00856290): RunningTimeSeconds less the start delay in seconds, clamped at zero.
#include <cstdio>
long g_framesPerSecond = 10;

class CCombatActionBase {
public:
    CCombatActionBase(long delay, float t) : Delay(delay), RunningTimeSeconds(t), LastRunningTimeSeconds(t) { }
    virtual ~CCombatActionBase() { }
    virtual long GetAnimDelayTime() const { return Delay; }
    float GetTimeThroughCurrentAnimation() const
    {
        long framesPerSecond = g_framesPerSecond;
        float t = RunningTimeSeconds - (float)GetAnimDelayTime() / framesPerSecond;
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
    if (a.GetTimeThroughCurrentAnimation() != 1.5f) { std::printf("00856290_TEST FAIL\n"); return 1; }
    CCombatActionBase b(30, 2.0f);                  // 2.0 - 3.0 < 0 -> 0
    if (b.GetTimeThroughCurrentAnimation() != 0.0f) { std::printf("00856290_TEST FAIL\n"); return 1; }
    g_framesPerSecond = 4;
    CCombatActionBase c(2, 1.0f);                   // 1.0 - 0.5
    if (c.GetTimeThroughCurrentAnimation() != 0.5f) { std::printf("00856290_TEST FAIL\n"); return 1; }
    std::printf("00856290_TEST PASS\n");
    return 0;
}