#include <cstdio>
// Behaviour copy of CAIStateGroup_MeleeCombatNearby2::Update (0x0093E5E0).
struct Sim { int enemyLife; bool combatMode; bool straightIntoStrafe; int moveFailAt; int moves; int attacks; int strafeAdds; int readied; int registered; int refreshed; int terminated; };
static void Update(Sim& s) {
    if (s.enemyLife <= 0) { ++s.terminated; return; }
    ++s.registered; ++s.refreshed;
    bool strafing = s.combatMode;
    if (s.straightIntoStrafe) { ++s.strafeAdds; strafing = false; }
    for (;;) {
        if (s.enemyLife <= 0) break;
        if (++s.moves == s.moveFailAt) { ++s.terminated; return; }
        if (strafing) { ++s.strafeAdds; strafing = false; }
        ++s.attacks; --s.enemyLife;
        ++s.readied;
    }
    ++s.terminated;
}
int main() {
    Sim a = { 3, true, false, 0, 0, 0, 0, 0, 0, 0, 0 };
    Update(a);
    if (a.attacks != 3 || a.strafeAdds != 1 || a.readied != 3 || a.terminated != 1 || a.registered != 1) { std::printf("0093e5e0_TEST FAIL\n"); return 1; }
    Sim b = { 0, true, false, 0, 0, 0, 0, 0, 0, 0, 0 };
    Update(b); if (b.registered != 0 || b.terminated != 1) { std::printf("0093e5e0_TEST FAIL\n"); return 1; }
    Sim c = { 5, true, true, 2, 0, 0, 0, 0, 0, 0, 0 };
    Update(c); if (c.attacks != 1 || c.strafeAdds != 1 || c.terminated != 1) { std::printf("0093e5e0_TEST FAIL\n"); return 1; }
    std::printf("0093e5e0_TEST PASS\n"); return 0;
}
