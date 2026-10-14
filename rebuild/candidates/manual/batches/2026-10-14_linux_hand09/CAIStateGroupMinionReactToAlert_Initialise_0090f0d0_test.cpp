#include <cstdio>
long g_FramesPerSecond = 15;
struct CBase { int baseCalls; long gotPriority; CBase() : baseCalls(0), gotPriority(0) { }
    virtual void Initialise(void* brain, void* creature, long priority, long scriptPriority, const void* name) { ++baseCalls; gotPriority = priority; } };
struct CGroup : CBase { unsigned long LastAlertReactedTo; long Timer;
    virtual void Initialise(void* brain, void* creature, long priority, long scriptPriority, const void* name)
    { CBase::Initialise(brain, creature, priority, scriptPriority, name); Timer = g_FramesPerSecond * 2; } };
int main()
{
    CGroup g; g.Timer = 0; g.Initialise(0, 0, 7, 1, 0);
    bool ok = g.baseCalls == 1 && g.gotPriority == 7 && g.Timer == 30;
    g_FramesPerSecond = 60; g.Initialise(0, 0, 1, 1, 0); ok = ok && g.Timer == 120;
    if (!ok) { std::printf("0090f0d0_TEST FAIL\n"); return 1; }
    std::printf("0090f0d0_TEST PASS\n"); return 0;
}
