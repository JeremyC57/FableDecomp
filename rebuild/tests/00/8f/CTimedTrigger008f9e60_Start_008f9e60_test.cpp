#include <cstdio>
static unsigned long g_tick = 5000;
static unsigned long FakeGetTickCount() { return g_tick; }
struct CTimedTrigger_008f9e60 {
    unsigned long m_startTick; long m_duration; int checks; long seenDuration;
    void CheckAndTrigger() { ++checks; seenDuration = m_duration; }
    void Start(long duration) { m_startTick = FakeGetTickCount(); m_duration = duration; CheckAndTrigger(); }
};
int main()
{
    CTimedTrigger_008f9e60 t; t.m_startTick = 0; t.m_duration = 0; t.checks = 0; t.seenDuration = 0;
    t.Start(250);
    if (t.m_startTick != 5000 || t.m_duration != 250 || t.checks != 1 || t.seenDuration != 250) { std::printf("008f9e60_TEST FAIL\n"); return 1; }
    std::printf("008f9e60_TEST PASS\n");
    return 0;
}