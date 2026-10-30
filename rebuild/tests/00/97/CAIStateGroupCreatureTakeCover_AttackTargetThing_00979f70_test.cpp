// behaviour test for 0x00979F70 CAIStateGroup_CreatureTakeCover::AttackTargetThing()
#include <cstdio>
struct World {
    bool runnable, targetAlive, canSee; int lookResult, intoAimResult, aimResult, fireResult;
    int terminates, unsheathes, speeches, fires; bool killAfterAim, killAfterFire;
};
static bool AttackTargetThing(World& w)
{
    ++w.unsheathes;
    if (!w.runnable) return false;
    if (w.lookResult != 2) return false;
    if (!w.targetAlive) return false;
    int result = w.intoAimResult;
    if (result != 2) { ++w.terminates; return false; }
    if (!w.targetAlive) return false;
    ++w.speeches;
    result = w.aimResult;
    if (w.killAfterAim) w.targetAlive = false;
    if (result != 2) { ++w.terminates; return false; }
    if (!w.targetAlive) { ++w.terminates; return false; }
    if (!w.canSee) return true;
    ++w.fires;
    result = w.fireResult;
    if (w.killAfterFire) w.targetAlive = false;
    if (result != 2) return false;
    if (!w.targetAlive) { ++w.terminates; return false; }
    return true;
}
int main()
{
    bool ok = true;
    World a = { true, true, true, 2, 2, 2, 2, 0, 0, 0, 0, false, false };
    ok = ok && AttackTargetThing(a) && a.fires == 1 && a.terminates == 0 && a.speeches == 1 && a.unsheathes == 1;
    World b = a; b.fires = 0; b.aimResult = 3;
    ok = ok && !AttackTargetThing(b) && b.terminates == 1 && b.fires == 0;
    World c = a; c.terminates = 0; c.killAfterAim = true;
    ok = ok && !AttackTargetThing(c) && c.terminates == 1;
    World d = a; d.terminates = 0; d.fires = 0; d.canSee = false;
    ok = ok && AttackTargetThing(d) && d.fires == 0;
    World e = a; e.terminates = 0; e.killAfterFire = true;
    ok = ok && !AttackTargetThing(e) && e.terminates == 1;
    World f = a; f.terminates = 0; f.runnable = false;
    ok = ok && !AttackTargetThing(f) && f.terminates == 0 && f.unsheathes == 2;
    World g = a; g.terminates = 0; g.intoAimResult = 1;
    ok = ok && !AttackTargetThing(g) && g.terminates == 1;
    std::printf(ok ? "00979f70_TEST PASS\n" : "00979f70_TEST FAIL\n");
    return ok ? 0 : 1;
}