#include <cstdio>
static long g_framesPerSecond = 30;
static int g_getterCalls = 0;
struct CWorldTime {
    static long SecondsToFrames(float seconds) { return (long)(g_framesPerSecond * seconds); }
};
static CWorldTime g_subsystem;
class CTCEmoteIcon { public:
    void* vptrSlot; void* ParentThing; bool m_flags[4];
    long ActiveTimer;
    CWorldTime* GetGameSubsystem1C() { ++g_getterCalls; return &g_subsystem; }
    void SetActiveTimer(float seconds)
    {
        if (seconds == -1.0f)
            ActiveTimer = -1;
        else if (seconds == 0.0f)
            ActiveTimer = 0;
        else
            ActiveTimer = GetGameSubsystem1C()->SecondsToFrames(seconds);
    }
};
int main()
{
    CTCEmoteIcon e; e.ActiveTimer = 7;
    e.SetActiveTimer(-1.0f); long a = e.ActiveTimer;
    e.SetActiveTimer(0.0f);  long b = e.ActiveTimer;
    int callsBefore = g_getterCalls;
    e.SetActiveTimer(2.0f);  long c = e.ActiveTimer;
    if (a != -1 || b != 0 || c != 60 || callsBefore != 0 || g_getterCalls != 1) { std::printf("0071aa50_TEST FAIL\n"); return 1; }
    std::printf("0071aa50_TEST PASS\n");
    return 0;
}