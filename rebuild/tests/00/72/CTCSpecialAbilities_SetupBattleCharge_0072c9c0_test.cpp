// Behaviour test for CTCSpecialAbilities::SetupBattleCharge (0x0072C9C0), g++ model of the retail logic.
#include <cstdio>
struct Thing { bool dead; unsigned typeFlags; };
struct Creature { bool assassinRush, battleCharge, hasTargeting; Thing* target; unsigned typeFlags; bool zTargeting;
                  int actions; Thing* actionTarget; bool actionZ; bool accept; };
static bool SetupBattleCharge(Creature& c)
{
    if (c.assassinRush || c.battleCharge) return false;
    if (c.hasTargeting) {
        Thing* t = c.target;
        if (t && !t->dead && (t->typeFlags & 8)) {
            bool z = false;
            if (c.typeFlags & 0x20) z = c.zTargeting;
            ++c.actions; c.actionTarget = t; c.actionZ = z; return c.accept;
        }
    }
    ++c.actions; c.actionTarget = 0; c.actionZ = false; return c.accept;
}
int main()
{
    Thing live = { false, 8 }, dead = { true, 8 }, plain = { false, 0 };
    Creature a = { true, false, true, &live, 0x20, true, 0, 0, false, true };
    int ok = !SetupBattleCharge(a) && a.actions == 0;
    Creature b = { false, false, true, &live, 0x20, true, 0, 0, false, true };
    ok &= SetupBattleCharge(b) && b.actionTarget == &live && b.actionZ;
    Creature c = { false, false, true, &live, 0, true, 0, 0, false, false };
    ok &= !SetupBattleCharge(c) && c.actions == 1 && c.actionTarget == &live && !c.actionZ;
    Creature d = { false, false, true, &dead, 0, false, 0, 0, false, true };
    ok &= SetupBattleCharge(d) && d.actionTarget == 0;
    Creature e = { false, false, true, &plain, 0, false, 0, 0, false, true };
    ok &= SetupBattleCharge(e) && e.actionTarget == 0;
    Creature f = { false, false, false, &live, 0, false, 0, 0, false, true };
    ok &= SetupBattleCharge(f) && f.actionTarget == 0 && f.actions == 1;
    std::printf(ok ? "0072c9c0_TEST PASS\n" : "0072c9c0_TEST FAIL\n");
    return ok ? 0 : 1;
}