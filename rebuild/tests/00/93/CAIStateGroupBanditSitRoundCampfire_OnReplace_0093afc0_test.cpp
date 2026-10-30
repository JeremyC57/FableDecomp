// behaviour test for 0x0093AFC0 CAIStateGroup_BanditSitRoundCampfire::OnReplace() (the seat/sit/stand-up flow and its Random step)
#include <cstdio>
static unsigned long ror13(unsigned long v) { return (v >> 13) | (v << 19); }
static unsigned long seedv = 12345, delayBase = 100, delayRange = 50;
static unsigned long Random(const unsigned long& range)
{
    unsigned long* seed = &seedv;
    *seed = *seed * 0x24a1 + 0x24df;
    unsigned long r = ror13(*seed);
    unsigned long n = range;
    *seed = r;
    return n ? r % n : 0;
}
struct State { bool gotSeat, campfireAlive, sitting, carrying, modeSit; long freedSeat; long seatIndex; unsigned long nextFrame; int standUps; int unsheathes; bool modeRemoved; };
static void OnReplace(State& s, unsigned long frame)
{
    if (s.gotSeat && s.campfireAlive) s.freedSeat = s.seatIndex;
    if (s.sitting || s.modeSit) {
        unsigned long delay = Random(delayRange);
        s.nextFrame = frame + delayBase + delay;
        ++s.standUps;
        s.modeRemoved = true;
        s.sitting = false;
    }
    if (s.carrying) ++s.unsheathes;
}
int main()
{
    unsigned long expectSeed = ror13(12345UL * 0x24a1 + 0x24df);
    State s = { true, true, true, true, false, -1, 3, 0, 0, 0, false };
    OnReplace(s, 1000);
    bool ok = s.freedSeat == 3 && !s.sitting && s.modeRemoved && s.standUps == 1 && s.unsheathes == 1
           && seedv == expectSeed && s.nextFrame == 1000 + 100 + expectSeed % 50;
    delayRange = 0;
    State t = { false, true, false, false, true, -1, 1, 0, 0, 0, false };
    OnReplace(t, 7);
    ok = ok && t.freedSeat == -1 && t.nextFrame == 107 && t.unsheathes == 0;
    std::printf(ok ? "0093afc0_TEST PASS\n" : "0093afc0_TEST FAIL\n");
    return ok ? 0 : 1;
}