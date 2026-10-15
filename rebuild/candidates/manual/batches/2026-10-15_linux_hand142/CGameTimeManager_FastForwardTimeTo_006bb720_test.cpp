#include <cstdio>
// Host model of CGameTimeManager::FastForwardTimeTo.
struct TimeManager {
    float Time; long DayCount; bool FastForwardingToTime; float TimeFastForwardingTo; float SecondsLeft;
    TimeManager() : Time(0.5f), DayCount(3), FastForwardingToTime(false), TimeFastForwardingTo(0), SecondsLeft(0) {}
    void FastForwardTimeTo(float time, float seconds)
    {
        if (time < 0.0f) time = 0.0f;
        else if (time > 1.0f) time = 1.0f;
        if (seconds > 0.0001f) { TimeFastForwardingTo = time; FastForwardingToTime = true; SecondsLeft = seconds; return; }
        if (time < Time) ++DayCount;
        Time = time;
    }
};
int main()
{
    int fail = 0;
    TimeManager m;
    m.FastForwardTimeTo(2.0f, 5.0f);
    if (!m.FastForwardingToTime || m.TimeFastForwardingTo != 1.0f || m.SecondsLeft != 5.0f || m.Time != 0.5f) fail = 1;
    TimeManager n;
    n.FastForwardTimeTo(0.75f, 0.0f);
    if (n.FastForwardingToTime || n.Time != 0.75f || n.DayCount != 3) fail = 1;
    n.FastForwardTimeTo(-1.0f, 0.0f);
    if (n.Time != 0.0f || n.DayCount != 4) fail = 1;
    if (fail) { std::printf("006bb720_TEST FAIL\n"); return 1; }
    std::printf("006bb720_TEST PASS\n");
    return 0;
}
