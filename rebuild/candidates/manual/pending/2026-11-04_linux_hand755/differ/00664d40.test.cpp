// Behaviour test for CThingCreatureBase::GetPCombatTypeDef (0x00664D40), g++ model of the parked source.
#include <cstdio>
#include <string>
struct Def { std::string name; };
static Def g_null = { "COMBAT_TYPE_NULL" }, g_byIndex = { "BY_INDEX" }, g_fromTC = { "FROM_TC" };
static const Def* Get(bool hasCombat, const Def* combatDef, long creatureDefIndex)
{
    if (hasCombat && combatDef) return combatDef;
    if (creatureDefIndex) return &g_byIndex;
    return &g_null;
}
int main()
{
    int ok = Get(true, &g_fromTC, 4) == &g_fromTC && Get(true, 0, 4) == &g_byIndex && Get(false, &g_fromTC, 4) == &g_byIndex;
    ok &= Get(false, 0, 0) == &g_null && Get(true, 0, 0) == &g_null;
    std::printf(ok ? "00664d40_TEST PASS\n" : "00664d40_TEST FAIL\n");
    return ok ? 0 : 1;
}
