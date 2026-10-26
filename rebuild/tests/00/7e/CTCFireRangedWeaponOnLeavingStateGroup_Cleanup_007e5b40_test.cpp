#include <cstdio>
#include <cmath>
// Behaviour test: CTCFireRangedWeaponOnLeavingStateGroup::Cleanup -- deregisters the creature from its combat manager, then removes creature mode 0x19 when it has a mode manager.
#define CHECK(c) if (!(c)) { std::printf("007e5b40_TEST FAIL\n"); return 1; }
class CThingCreatureBase;
class CCombatManager { public: CThingCreatureBase* last; int count; void DeregisterCombatant(CThingCreatureBase* c) { last = c; ++count; } };
class CTCCreatureModeManager { public: int removed; void RemoveMode(int mode) { removed = mode; } };
class CThingCreatureBase { public:
    CCombatManager* Combat;
    CTCCreatureModeManager* Modes;
    CCombatManager* GetCombatManager() { return Combat; }
    bool PeekTC(CTCCreatureModeManager** out) { if (Modes) { *out = Modes; return true; } return false; }
};
static void Cleanup(CThingCreatureBase& creature)
{
    creature.GetCombatManager()->DeregisterCombatant(&creature);
    CTCCreatureModeManager* modes;
    if (creature.PeekTC(&modes))
        modes->RemoveMode(0x19);
}
int main()
{
    CCombatManager combat; combat.last = 0; combat.count = 0;
    CTCCreatureModeManager modes; modes.removed = 0;
    CThingCreatureBase a; a.Combat = &combat; a.Modes = 0;
    Cleanup(a);
    CHECK(combat.last == &a && combat.count == 1 && modes.removed == 0)
    CThingCreatureBase b; b.Combat = &combat; b.Modes = &modes;
    Cleanup(b);
    CHECK(combat.last == &b && combat.count == 2 && modes.removed == 0x19)
    std::printf("007e5b40_TEST PASS\n");
    return 0;
}