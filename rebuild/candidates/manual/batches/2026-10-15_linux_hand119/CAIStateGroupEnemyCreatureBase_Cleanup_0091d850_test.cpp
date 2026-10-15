// Behaviour test for 0x0091D850: EnemyCreatureBase::Cleanup.
#include <cstdio>
struct CCreature { bool registered; bool IsRegisteredCombatant() { return registered; } };
struct CCombatManager { CCreature* removed; void DeregisterCombatant(CCreature* c) { removed = c; } };
struct CGroup {
    CCreature* creature; CCombatManager* mgr; void* PEnemy; int hooks;
    void OnCleanup() { ++hooks; }
    void Cleanup()
    {
        if (creature->IsRegisteredCombatant()) {
            CCreature* c = creature;
            CCombatManager* m = mgr;
            m->DeregisterCombatant(c);
        }
        OnCleanup();
        PEnemy = 0;
    }
};
int main()
{
    CCreature c = { true }; CCombatManager m = { 0 }; int enemy = 0;
    CGroup g = { &c, &m, &enemy, 0 };
    g.Cleanup();
    int bad = (m.removed != &c || g.hooks != 1 || g.PEnemy != 0);
    m.removed = 0; c.registered = false; g.PEnemy = &enemy; g.Cleanup();
    if (m.removed != 0 || g.hooks != 2 || g.PEnemy != 0) bad = 2;
    if (bad) { std::printf("0091d850_TEST FAIL\n"); return 1; }
    std::printf("0091d850_TEST PASS\n");
    return 0;
}
