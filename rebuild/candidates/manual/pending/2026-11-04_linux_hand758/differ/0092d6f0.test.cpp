// Behaviour test for CAIStateGroup_GoToBed::IsRunnable (0x0092D6F0), g++ model of the runnable test.
#include <cstdio>
struct State { bool bedAlive, hasSleep; long frame, wokeUp; bool crimeKnown, trespass, sleepTime, freeBed, banditBed; int releases; };
static bool IsRunnable(State& s)
{
    if (s.bedAlive) ++s.releases;
    if (s.hasSleep && s.frame - s.wokeUp < 120) return false;
    if (s.crimeKnown || s.trespass || !s.sleepTime) return false;
    if (s.freeBed || s.banditBed) { ++s.releases; return true; }
    return false;
}
int main()
{
    State a = { true, true, 1000, 500, false, false, true, false, true, 0 };
    int ok = IsRunnable(a) && a.releases == 2;
    State b = a; b.releases = 0; b.wokeUp = 950; ok = ok && !IsRunnable(b) && b.releases == 1;
    State c = a; c.releases = 0; c.sleepTime = false; ok = ok && !IsRunnable(c);
    State d = a; d.releases = 0; d.bedAlive = false; d.banditBed = false; ok = ok && !IsRunnable(d) && d.releases == 0;
    State e = a; e.hasSleep = false; e.wokeUp = 999; e.releases = 0; ok = ok && IsRunnable(e);
    std::printf(ok ? "0092d6f0_TEST PASS\n" : "0092d6f0_TEST FAIL\n");
    return ok ? 0 : 1;
}
