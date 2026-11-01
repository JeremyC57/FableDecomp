// Behaviour test for CAIStateGroup_EnemyCreatureBase::OnActivate (0x0091D8C0): living target -> (combat group: register the
// creature as the target's combatant) then OnActivateLeaf; no living target -> only the IsCombatStateGroup query.
#include <cstdio>
class CThing { public: bool Dead; bool IsDead() const { return Dead; } };
class CCombatManager { public:
    int Registered; CThing* Attacker; CThing* Target; bool Flag;
    void RegisterCombatant(CThing& attacker, CThing& target, bool flag) { ++Registered; Attacker = &attacker; Target = &target; Flag = flag; }
};
struct CPointer { CThing* Item; CThing* Get() const { return Item; } CThing* operator->() const { return Item; } CThing& operator*() const { return *Item; } };
inline bool IS_THING_ALIVE(const CPointer& thing) { return thing.Get() && !thing->IsDead(); }
struct CGroup {
    CCombatManager Manager;
    CThing* Creature;
    CPointer PTargetThing;
    bool Combat;
    int CombatQueries, LeafCalls;
    CCombatManager& GetCombatManager() { return Manager; }
    CThing& GetCreature() const { return *Creature; }
    bool IsCombatStateGroup() { ++CombatQueries; return Combat; }
    void OnActivateLeaf() { ++LeafCalls; }
    void OnActivate()
    {
        if (IS_THING_ALIVE(PTargetThing)) {
            if (IsCombatStateGroup())
                GetCombatManager().RegisterCombatant(GetCreature(), *PTargetThing, true);
            OnActivateLeaf();
        } else
            IsCombatStateGroup();
    }
};
static void Reset(CGroup& g, CThing* creature, CThing* target, bool combat)
{
    g.Manager.Registered = 0; g.Manager.Attacker = 0; g.Manager.Target = 0; g.Manager.Flag = false;
    g.Creature = creature; g.PTargetThing.Item = target; g.Combat = combat; g.CombatQueries = 0; g.LeafCalls = 0;
}
int main()
{
    CThing me = { false }, foe = { false }, corpse = { true };
    CGroup g;
    int fail = 0;
    Reset(g, &me, &foe, true); g.OnActivate();
    if (g.Manager.Registered != 1 || g.Manager.Attacker != &me || g.Manager.Target != &foe || !g.Manager.Flag || g.LeafCalls != 1) fail = 1;
    Reset(g, &me, &foe, false); g.OnActivate();
    if (g.Manager.Registered != 0 || g.LeafCalls != 1 || g.CombatQueries != 1) fail = 1;
    Reset(g, &me, &corpse, true); g.OnActivate();
    if (g.Manager.Registered != 0 || g.LeafCalls != 0 || g.CombatQueries != 1) fail = 1;
    Reset(g, &me, 0, true); g.OnActivate();
    if (g.Manager.Registered != 0 || g.LeafCalls != 0 || g.CombatQueries != 1) fail = 1;
    if (fail) { std::printf("0091d8c0_TEST FAIL\n"); return 1; }
    std::printf("0091d8c0_TEST PASS\n");
    return 0;
}