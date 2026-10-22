#include <cstdio>
#include <cstring>
static char g_log[8192];
static int g_frame = 0;
static int g_termAt = 1000;
static void Log(const char* s) { char b[32]; std::sprintf(b, "@%d;", g_frame); std::strcat(g_log, s); std::strcat(g_log, b); }
static void Reset(int termAt) { g_log[0] = 0; g_frame = 0; g_termAt = termAt; }
static int Fail(const char* addr, const char* what) { std::printf("%s_TEST FAIL %s [%s]\n", addr, what, g_log); return 1; }
#define CHECK(addr, cond) do { if (!(cond)) return Fail(addr, #cond); } while (0)
// behaviour test: CBountyHuntActivationChecker::RespawnFishermen creates the organiser and (unless attacked) both competitors
struct C3DVector { float X, Y, Z; };
struct CScriptThing {
    const char* Name; float X; char Data[8];
    CScriptThing(const char* n = "", float x = 0.0f) : Name(n), X(x) { Data[0] = 0; }
    C3DVector GetPos() const { C3DVector v; v.X = X; v.Y = v.Z = 0.0f; return v; }
    void SetDataString(const char* s) { std::strcpy(Data, s); char x[64]; std::sprintf(x, "data %s %s", Name, s); Log(x); }
};
static int g_n = 0;
struct CGame {
    CScriptThing GetThingWithScriptName(const char* n) const { return CScriptThing(n, n[std::strlen(n) - 1] == '2' ? 2.0f : 1.0f); }
    CScriptThing CreateCreature(const char* def, const C3DVector& p, const char* name, bool) const
    { char x[128]; std::sprintf(x, "create %s %s %g", def, name, p.X); Log(x); static const char* ids[] = { "A", "B", "C", "D" }; return CScriptThing(ids[g_n++ % 4]); }
    void EntitySetAppearanceMorphSeed(const CScriptThing& t, long s) const { char x[32]; std::sprintf(x, "seed %s %ld", t.Name, s); Log(x); }
};
struct CV_FishingCompetitionScript { bool CompetitorsAttacked; };
struct CBountyHuntActivationChecker {
    CGame* Game; CV_FishingCompetitionScript* ParentClass;
    void RespawnFishermen();
};
void CBountyHuntActivationChecker::RespawnFishermen()
{
    Game->CreateCreature("CREATURE_FISHERMAN", Game->GetThingWithScriptName("MK_FC_ORG").GetPos(), "FishCompOrganiser", false);
    if (!ParentClass->CompetitorsAttacked) {
        CScriptThing competitor1 = Game->CreateCreature("CREATURE_BOWERSTONE_SLUMS_VILLAGER_MALE_WAREHOUSE_WORKER", Game->GetThingWithScriptName("MK_FI_COMP1").GetPos(), "FishCompCompetitor", false);
        CScriptThing competitor2 = Game->CreateCreature("CREATURE_BOWERSTONE_SLUMS_VILLAGER_MALE_UNEMPLOYED", Game->GetThingWithScriptName("MK_FI_COMP2").GetPos(), "FishCompCompetitor", false);
        competitor1.SetDataString("1");
        competitor2.SetDataString("2");
        Game->EntitySetAppearanceMorphSeed(competitor1, 9);
        Game->EntitySetAppearanceMorphSeed(competitor2, 36);
    }
}
int main()
{
    CGame game; CV_FishingCompetitionScript p; CBountyHuntActivationChecker c; c.Game = &game; c.ParentClass = &p;
    Reset(100); g_n = 0; p.CompetitorsAttacked = false;
    c.RespawnFishermen();
    CHECK("00e7f7f0", std::strcmp(g_log, "create CREATURE_FISHERMAN FishCompOrganiser 1@0;"
        "create CREATURE_BOWERSTONE_SLUMS_VILLAGER_MALE_WAREHOUSE_WORKER FishCompCompetitor 1@0;"
        "create CREATURE_BOWERSTONE_SLUMS_VILLAGER_MALE_UNEMPLOYED FishCompCompetitor 2@0;"
        "data B 1@0;data C 2@0;seed B 9@0;seed C 36@0;") == 0);
    Reset(100); g_n = 0; p.CompetitorsAttacked = true;
    c.RespawnFishermen();
    CHECK("00e7f7f0", std::strcmp(g_log, "create CREATURE_FISHERMAN FishCompOrganiser 1@0;") == 0);
    std::printf("00e7f7f0_TEST PASS\n");
    return 0;
}
