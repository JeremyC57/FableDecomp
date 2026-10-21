// Behaviour test for 0x007A71B0 (special-ability TC constructor): def looked up by "SPECIAL_ABILITIES_BATTLE_CHARGE_DEF", a creature
// parent's spell-cast state set to mode 12 and its charge mode stopped, the spell level read for ability 12.
#include <cstdio>
#include <cstring>

static char g_defName[64]; static int g_stop = 0, g_reset = 0, g_finish = 0;
struct State { bool Active; long Mode; };
struct Thing {
    unsigned char ThingType; bool hasState; State state; bool hasAbilities; long level;
    void StopChargeMode() { ++g_stop; }
    long GetAbilityLevel(long ability) { return ability == 12 ? level : -1; }
    void FinishCurrentAction() { ++g_finish; }
};
static long GetGlobalIndexFromClassIndex(const char* name, long classIndex) { std::strcpy(g_defName, name); return classIndex + 40; }
struct TC {
    Thing* ParentThing; long PDef; long SpellLevel;
    TC(Thing* parent) : ParentThing(parent), PDef(0), SpellLevel(0)
    {
        long index = GetGlobalIndexFromClassIndex("SPECIAL_ABILITIES_BATTLE_CHARGE_DEF", 1);
        PDef = index;
        if (ParentThing->ThingType == 2) {
            if (ParentThing->hasState) { ParentThing->state.Active = true; ParentThing->state.Mode = 12; }
            ParentThing->StopChargeMode();
        }
        if (ParentThing->hasAbilities)
            SpellLevel = ParentThing->GetAbilityLevel(12);
        ++g_reset;
        if (1) ParentThing->FinishCurrentAction();
    }
};
int main()
{
    Thing creature = { 2, true, { false, 0 }, true, 3 };
    Thing object = { 1, true, { false, 0 }, false, 9 };
    TC a(&creature);
    TC b(&object);
    bool ok = std::strcmp(g_defName, "SPECIAL_ABILITIES_BATTLE_CHARGE_DEF") == 0 && a.PDef == 41 && creature.state.Active
        && creature.state.Mode == 12 && g_stop == 1 && a.SpellLevel == 3 && !object.state.Active && b.SpellLevel == 0
        && g_reset == 2 && g_finish == (1 ? 2 : 0);
    if (!ok) { std::printf("007a71b0_TEST FAIL\n"); return 1; }
    std::printf("007a71b0_TEST PASS\n");
    return 0;
}