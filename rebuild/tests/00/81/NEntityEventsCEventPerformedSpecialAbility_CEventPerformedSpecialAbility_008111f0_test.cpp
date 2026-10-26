// Behaviour test for 0x008111F0 CEventPerformedSpecialAbility(EHeroAbility) (model copy with a stub definition manager).
#include <cstdio>
#include <cstring>
class CHeroAbilityDef { public: int RefCount; int released; long Ability; bool Aggressive; };
static CHeroAbilityDef g_defs[5];
static int g_lookups = 0, g_badType = 0;
struct DefPtr {
    CHeroAbilityDef* Object;
    DefPtr() : Object(0) { }
    ~DefPtr() { if (Object && --Object->RefCount == 0) Object->released++; }
};
struct Manager {
    long GetNumberOfDefsOfType(const char* type) { if (std::strcmp(type, "HERO_ABILITY") != 0) ++g_badType; return 5; }
    void GetDef(const char* type, long index, DefPtr& out)
    { if (std::strcmp(type, "HERO_ABILITY") != 0) ++g_badType; ++g_lookups; out.Object = &g_defs[index]; ++g_defs[index].RefCount; }
};
static Manager g_mgr;
struct Event { long WorldFrame; long Ability; bool Aggressive; };
static void construct_model(Event* e, long ability)
{
    e->WorldFrame = 0; e->Ability = ability; e->Aggressive = true;
    Manager& manager = g_mgr;
    long count = manager.GetNumberOfDefsOfType("HERO_ABILITY");
    for (long i = 1; i < count; ++i) {
        DefPtr def;
        manager.GetDef("HERO_ABILITY", i, def);
        if (def.Object->Ability == e->Ability) { e->Aggressive = def.Object->Aggressive; return; }
    }
}
int main()
{
    int i, ok = 1;
    for (i = 0; i < 5; ++i) { g_defs[i].RefCount = 1; g_defs[i].released = 0; g_defs[i].Ability = 100 + i; g_defs[i].Aggressive = (i % 2) == 1; }
    Event e;
    construct_model(&e, 102);                 // found at index 2 (not aggressive)
    if (e.WorldFrame != 0 || e.Ability != 102 || e.Aggressive != false || g_lookups != 2) ok = 0;
    for (i = 0; i < 5; ++i) if (g_defs[i].RefCount != 1) ok = 0;
    g_lookups = 0;
    construct_model(&e, 100);                 // index 0 is never searched -> stays aggressive
    if (e.Aggressive != true || g_lookups != 4) ok = 0;
    g_lookups = 0;
    construct_model(&e, 103);
    if (e.Aggressive != true || g_lookups != 3) ok = 0;
    if (g_badType != 0) ok = 0;
    if (!ok) { std::printf("008111f0_TEST FAIL\n"); return 1; }
    std::printf("008111f0_TEST PASS\n");
    return 0;
}