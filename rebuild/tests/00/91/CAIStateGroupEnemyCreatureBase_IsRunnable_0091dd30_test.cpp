// Behaviour test for 0x0091DD30: EnemyCreatureBase::IsRunnable.
#include <cstdio>
struct CCreature { int id; };
struct CGroup {
    CCreature* found; CCreature* PEnemy; bool valid;
    CCreature* GetEnemy() { return found; }
    bool IsStateGroupStillValid() { return valid; }
    bool IsRunnable()
    {
        CCreature* enemy = GetEnemy();
        if (enemy != 0) {
            PEnemy = enemy;
            if (IsStateGroupStillValid()) return true;
        }
        return false;
    }
};
int main()
{
    CCreature e = { 1 };
    CGroup g = { &e, 0, true };
    int bad = 0;
    if (!g.IsRunnable() || g.PEnemy != &e) bad = 1;
    g.valid = false; if (g.IsRunnable()) bad = 2;
    g.found = 0; g.valid = true; g.PEnemy = 0; if (g.IsRunnable() || g.PEnemy != 0) bad = 3;
    if (bad) { std::printf("0091dd30_TEST FAIL\n"); return 1; }
    std::printf("0091dd30_TEST PASS\n");
    return 0;
}