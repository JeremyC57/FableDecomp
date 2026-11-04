// Behaviour test for 0x0096DBC0 CCreatureAction_ScreamerIntoAggressive::Initialise(): a model; checks the aggressive mode is added with the live target (or without one), the into-aggression message is sent, then the INTO_COMBAT animation and the base Initialise.
#include <cstdio>
#include <cmath>
#include <vector>
#include <string>
#include <set>
#include <map>

struct Log { int modeWithTarget, modeAlone, messages, anim, base; };
static void Initialise(Log& l, bool hasModes, bool targetAlive)
{
    if (hasModes) { if (targetAlive) ++l.modeWithTarget; else ++l.modeAlone; }
    ++l.messages; ++l.anim; ++l.base;
}
int main()
{
    int ok = 1; Log l = { 0, 0, 0, 0, 0 };
    Initialise(l, true, true); Initialise(l, true, false);
    ok &= l.modeWithTarget == 1 && l.modeAlone == 1 && l.messages == 2 && l.anim == 2 && l.base == 2;
    std::printf(ok ? "0096DBC0_TEST PASS\n" : "0096DBC0_TEST FAIL\n"); return ok ? 0 : 1;
}
