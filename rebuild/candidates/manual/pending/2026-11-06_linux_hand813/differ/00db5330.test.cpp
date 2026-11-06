// Behaviour test for CNOVI_BarrelMan::Main (0x00DB5330): the per-frame step of the Oakvale barrel man -- the MyPhase steps 1..4
// (walk off, wait for the watch timer to reach 15, return, then thank or complain depending on whether the hero stayed), the hit
// complaint, the talk / notice answers by phase (favour hands over the warehouse and moves to phase 2), and the overhear line.
#include <cstdio>
#include <cstring>

struct BarrelMan {
    long MyPhase, WatchTimer, GoodDeeds, BadDeeds, Overheard;
    bool HeroLetMeDown, OverheardYet, LeftHeroInCharge, BarrelBroken, SpokenOnReturn;
    char Last[48];
    void Say(const char* t) { std::strcpy(Last, t); }
    void Step(bool hit, bool talked, float heroDist, bool heroSawMe, int roll)
    {
        if (MyPhase != 0) {
            switch (MyPhase) {
            case 1: WatchTimer = 45; MyPhase = 2; break;
            case 2: if (WatchTimer == 15) MyPhase = 3; break;
            case 3: MyPhase = 4; break;
            case 4:
                SpokenOnReturn = true;
                if (!heroSawMe && heroDist >= 10.0f) { Say("WHERE_GONE"); HeroLetMeDown = true; ++BadDeeds; }
                else { Say("THANKS"); ++GoodDeeds; }
                MyPhase = 5;
                break;
            }
        }
        if (hit) { ++BadDeeds; Say("CAREFUL"); }
        else if ((MyPhase == 0 && heroDist < 10.0f) || talked) {
            if (MyPhase == 0) { Say("FAVOUR"); MyPhase = 2; WatchTimer = 45; LeftHeroInCharge = true; }
            else if (MyPhase < 4) Say("NOT_LARKING");
            else if (MyPhase == 5) {
                if (HeroLetMeDown) Say(BarrelBroken ? "LETDOWN_BROKEN" : "LETDOWN_NOT_BROKE");
                else Say("NO_TIME");
            }
        } else if (MyPhase == 0 && (!OverheardYet || roll == 0) && heroDist < 15.0f) {
            OverheardYet = true; ++Overheard; Say("OVERHEAR");
        }
    }
};

int main()
{
    BarrelMan b; std::memset(&b, 0, sizeof b);
    bool ok = true;
    b.Step(false, false, 12.0f, true, 3);
    ok = ok && b.Overheard == 1 && b.OverheardYet;
    b.Step(false, false, 12.0f, true, 3);
    ok = ok && b.Overheard == 1;
    b.Step(false, false, 12.0f, true, 0);
    ok = ok && b.Overheard == 2;
    b.Step(false, false, 5.0f, true, 3);
    ok = ok && std::strcmp(b.Last, "FAVOUR") == 0 && b.MyPhase == 2 && b.WatchTimer == 45 && b.LeftHeroInCharge;
    b.Step(false, true, 5.0f, true, 3);
    ok = ok && std::strcmp(b.Last, "NOT_LARKING") == 0 && b.MyPhase == 2;
    b.WatchTimer = 15;
    b.Step(false, false, 30.0f, false, 3);
    ok = ok && b.MyPhase == 3;
    b.Step(false, false, 30.0f, false, 3);
    b.Step(false, false, 30.0f, false, 3);
    ok = ok && b.MyPhase == 5 && b.HeroLetMeDown && b.BadDeeds == 1 && b.SpokenOnReturn;
    b.BarrelBroken = true;
    b.Step(false, true, 3.0f, true, 3);
    ok = ok && std::strcmp(b.Last, "LETDOWN_BROKEN") == 0;
    b.Step(true, true, 3.0f, true, 3);
    ok = ok && std::strcmp(b.Last, "CAREFUL") == 0 && b.BadDeeds == 2;
    BarrelMan c; std::memset(&c, 0, sizeof c); c.MyPhase = 4;
    c.Step(false, true, 3.0f, true, 3);
    ok = ok && c.GoodDeeds == 1 && c.MyPhase == 5 && std::strcmp(c.Last, "NO_TIME") == 0;
    if (!ok) { std::printf("00db5330_TEST FAIL\n"); return 1; }
    std::printf("00db5330_TEST PASS\n");
    return 0;
}
