// behaviour test for 0x00DD26E0 CQ_PrisonEscapeRescueMotherScript::CQ_PrisonEscapeRescueMotherScript: the script base is built first, Game and MasterScriptData take the arguments, and the RunningTimer timer is registered once, last
#include <cstdio>
static int gStep = 0, gBaseAt = -1, gTimerAt = -1, gTimers = 0;
struct Script { const void* game; const void* master; long timer; };
static long RegisterTimer() { ++gTimers; gTimerAt = gStep++; return 41 + gTimers; }
static void Construct(Script& s, const void* data, const void* game)   // the function under test
{
    gBaseAt = gStep++;                                       // CScriptBase::CScriptBase
    s.game = game; s.master = data;                          // CLevelScriptBase members
    s.timer = RegisterTimer();                               // CTimer member
}
int main()
{
    int fail = 0;
    int data = 1, game = 2;
    Script s; s.game = 0; s.master = 0; s.timer = 0;
    Construct(s, &data, &game);
    if (gBaseAt != 0 || gTimerAt != 1 || gTimers != 1) fail |= 1;
    if (s.game != &game || s.master != &data || s.timer != 42) fail |= 2;
    if (fail) { std::printf("00dd26e0_TEST FAIL %d\n", fail); return 1; }
    std::printf("00dd26e0_TEST PASS\n");
    return 0;
}