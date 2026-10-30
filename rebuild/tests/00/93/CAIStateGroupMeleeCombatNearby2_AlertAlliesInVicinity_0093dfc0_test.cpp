// behaviour test for 0x0093DFC0 CAIStateGroup_MeleeCombatNearby2::AlertAlliesInVicinity (alert once when perceiving the enemy)
#include <cstdio>
struct Perceive { void* perceived; bool LineOfSight, Smelt; };
struct World { int messages12; int added; };
static bool Alert(Perceive* p, void* enemy, World* w)
{
    if (!p) return true;
    if (p->perceived != enemy || (!p->LineOfSight && !p->Smelt)) return true;
    if (w->messages12) return true;
    ++w->added; ++w->messages12;
    return true;
}
int main()
{
    int enemy, other;
    World w = { 0, 0 };
    Perceive seen = { &enemy, true, false }, smelt = { &enemy, false, true }, blind = { &enemy, false, false }, wrong = { &other, true, true };
    bool ok = Alert(0, &enemy, &w) && w.added == 0;
    ok = ok && Alert(&blind, &enemy, &w) && w.added == 0;
    ok = ok && Alert(&wrong, &enemy, &w) && w.added == 0;
    ok = ok && Alert(&seen, &enemy, &w) && w.added == 1;
    ok = ok && Alert(&smelt, &enemy, &w) && w.added == 1;
    w.messages12 = 0; ok = ok && Alert(&smelt, &enemy, &w) && w.added == 2;
    std::printf(ok ? "0093dfc0_TEST PASS\n" : "0093dfc0_TEST FAIL\n");
    return ok ? 0 : 1;
}