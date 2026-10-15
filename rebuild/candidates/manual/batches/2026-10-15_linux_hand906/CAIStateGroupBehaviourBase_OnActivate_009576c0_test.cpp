#include <cstdio>
// Behaviour test for CAIStateGroupBehaviourBase::OnActivate (0x009576C0): clears the three timers, then runs the hook.
static int g_hook = 0;
struct CAIStateGroupBehaviourBase {
    virtual void v00() {} virtual void v01() {}
    virtual void OnActivate() { TriggerTime = 0; ThisSpeechDuration = 0; RandomWaitTime = 0; OnBehaviourActivated(); }
    virtual void v03() {} virtual void v04() {} virtual void v05() {} virtual void v06() {} virtual void v07() {}
    virtual void v08() {} virtual void v09() {} virtual void v10() {} virtual void v11() {} virtual void v12() {}
    virtual void v13() {} virtual void v14() {} virtual void v15() {}
    virtual void OnBehaviourActivated() { ++g_hook; }
    float TriggerTime, RandomWaitTime, ThisSpeechDuration;
};
struct CDerived : CAIStateGroupBehaviourBase { void OnBehaviourActivated() { g_hook += 10; } };
int main()
{
    CDerived d; d.TriggerTime = 1.5f; d.RandomWaitTime = 2.5f; d.ThisSpeechDuration = 3.5f;
    d.OnActivate();
    if (d.TriggerTime != 0.0f || d.RandomWaitTime != 0.0f || d.ThisSpeechDuration != 0.0f || g_hook != 10) { std::printf("009576c0_TEST FAIL\n"); return 1; }
    std::printf("009576c0_TEST PASS\n");
    return 0;
}
