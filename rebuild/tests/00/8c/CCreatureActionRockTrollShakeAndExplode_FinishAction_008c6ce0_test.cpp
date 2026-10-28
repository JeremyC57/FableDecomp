// Behaviour test: CCreatureAction_RockTrollShakeAndExplode::FinishAction (0x008C6CE0) -- explode with the def's index
// (immune creature) only when the troll battle def exists and the index is positive, then finish and kill the creature.
#include <cstdio>
#include <cstring>
static char g_log[256];
static void Log(const char* s) { std::strcat(g_log, s); std::strcat(g_log, ";"); }
class CTrollBattleDef { public: int DeathExplosionIndex; };
class CThing { public:
    bool HasExplosionTC;
    bool Killed;
    bool Immune;
    bool HasTC(int type) const { return type == 0x41 && HasExplosionTC; }
    void Kill(bool) { Killed = true; Log("kill"); }
};
static CThing g_explosion;
static bool g_returnExplosion;
static int g_createdIndex;
static CThing* CreateExplosion(int index) { g_createdIndex = index; Log("create"); return g_returnExplosion ? &g_explosion : 0; }
struct CCreature : CThing { const CTrollBattleDef* Def; };
static void FinishAction(CCreature* creature)
{
    const CTrollBattleDef* def = creature->Def;
    if (def != 0 && def->DeathExplosionIndex > 0) {
        CThing* explosion = CreateExplosion(def->DeathExplosionIndex);
        if (explosion && explosion->HasTC(0x41)) { creature->Immune = true; Log("immune"); }
    }
    Log("base");
    creature->Kill(true);
}
static bool Run(const CTrollBattleDef* def, bool create, bool tc, const char* expect)
{
    g_log[0] = 0; g_returnExplosion = create; g_explosion.HasExplosionTC = tc;
    CCreature c; std::memset(&c, 0, sizeof(c)); c.Def = def;
    FinishAction(&c);
    return std::strcmp(g_log, expect) == 0 && c.Killed;
}
int main()
{
    CTrollBattleDef d5 = { 5 }, d0 = { 0 };
    if (!Run(0, true, true, "base;kill;") || !Run(&d0, true, true, "base;kill;")
        || !Run(&d5, true, true, "create;immune;base;kill;") || g_createdIndex != 5
        || !Run(&d5, true, false, "create;base;kill;") || !Run(&d5, false, true, "create;base;kill;")) {
        std::printf("008c6ce0_TEST FAIL\n"); return 1;
    }
    std::printf("008c6ce0_TEST PASS\n");
    return 0;
}