#include <cstdio>
static long Ticks() { return 4242; }
long (*g_pfnGetTickCount)() = Ticks;
struct CMusicTimer_008fa000 { unsigned char _pad[0x100]; long m_startTicks; long m_elapsed; float m_rate; long m_triggered; int checks;
    void CheckAndTrigger() { ++checks; } CMusicTimer_008fa000* Reset(); };
CMusicTimer_008fa000* CMusicTimer_008fa000::Reset() { m_rate = 1.0f; m_triggered = 0; m_startTicks = g_pfnGetTickCount(); m_elapsed = 0; CheckAndTrigger(); CheckAndTrigger(); return this; }
int main()
{
    CMusicTimer_008fa000 t; t.checks = 0; t.m_rate = 0; t.m_triggered = 9; t.m_startTicks = 0; t.m_elapsed = 3;
    if (t.Reset() != &t || t.m_rate != 1.0f || t.m_triggered || t.m_startTicks != 4242 || t.m_elapsed || t.checks != 2) { std::printf("008fa000_TEST FAIL\n"); return 1; }
    std::printf("008fa000_TEST PASS\n"); return 0;
}
