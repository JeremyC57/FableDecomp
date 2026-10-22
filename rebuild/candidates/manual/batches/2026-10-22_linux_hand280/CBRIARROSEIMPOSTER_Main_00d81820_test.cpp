// Behaviour test for CBRIAR_ROSE_IMPOSTER::Main (0x00D81820): a hero hit (or any special ability but 14) with the taunt timer expired runs the taunt conversation and restarts both timers; the "here" line needs the here timer expired, the taunt timer above 7 and the hero over 13 units away.
#include <cstdio>
#include <cstring>
struct CTimer { long t; long GetTime() const { return t; } void SetTime(long v) { t = v; } };
struct CImposter {
    CTimer Taunt, Here; int taunts, heres;
    void Step(bool hitBy, bool anySpecial, bool special14, float distance)
    {
        bool hit = (hitBy || (anySpecial && !special14)) && Taunt.GetTime() == 0;
        if (hit) { ++taunts; Taunt.SetTime(15); Here.SetTime(4); }
        if (Here.GetTime() == 0 && Taunt.GetTime() > 7 && distance > 13.0f) { ++heres; Here.SetTime(15); }
    }
};
int main()
{
    CImposter m; m.taunts = m.heres = 0; m.Taunt.SetTime(0); m.Here.SetTime(4);
    m.Step(true, false, false, 5.0f);
    bool ok = m.taunts == 1 && m.Taunt.t == 15 && m.Here.t == 4;
    m.Step(true, false, false, 20.0f);
    ok = ok && m.taunts == 1 && m.heres == 0;
    m.Here.t = 0;
    m.Step(false, false, false, 20.0f);
    ok = ok && m.heres == 1 && m.Here.t == 15;
    m.Taunt.t = 0; m.Here.t = 0;
    m.Step(false, true, true, 1.0f);
    ok = ok && m.taunts == 1;
    m.Step(false, true, false, 1.0f);
    ok = ok && m.taunts == 2 && m.heres == 1;
    if (!ok) { std::printf("00d81820_TEST FAIL\n"); return 1; }
    std::printf("00d81820_TEST PASS\n");
    return 0;
}
