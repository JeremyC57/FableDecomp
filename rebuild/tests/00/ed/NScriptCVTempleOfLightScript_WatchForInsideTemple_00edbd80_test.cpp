// Behaviour test for CV_TempleOfLightScript::WatchForInsideTemple (0x00EDBD80).
#include <cstdio>
#include <cstring>
static char g_log[1024];
static void Log(const char* s) { std::strcat(g_log, s); std::strcat(g_log, ";"); }
struct CScriptThing {
    const char* def;    // 0 = null thing
    bool alive;
    bool IsNull() const { return def == 0; }
    bool IsAlive() const { return alive; }
    const char* GetDefName() const { return def ? def : ""; }
};
struct CGameScriptInterface {
    int frame; const char* const* bld; const bool* alive; CScriptThing hero;
    CScriptThing& GetHero() { hero.alive = alive[frame]; hero.def = "HERO"; return hero; }
    CScriptThing EntityGetStandingInsideBuilding(const CScriptThing&) { CScriptThing t; t.def = bld[frame]; t.alive = true; return t; }
    void NewScriptFrame() { Log("frame"); ++frame; }
};
struct CV_TempleOfLightScript {
    CGameScriptInterface* Game; bool InsideTemple; int termAfter;
    bool IsActiveThreadTerminating() { return --termAfter < 0; }
    void WatchForInsideTemple()
    {
        CScriptThing building; building.def = 0;
        for (;;) {
            if (IsActiveThreadTerminating()) return;
            if (Game->GetHero().IsAlive()) {
                if (IsActiveThreadTerminating()) return;
                building = Game->EntityGetStandingInsideBuilding(Game->GetHero());
                if (!building.IsNull() && std::strcmp(building.GetDefName(), "BUILDING_CHAPEL_GOOD") == 0 && !InsideTemple) {
                    if (IsActiveThreadTerminating()) return;
                    InsideTemple = true; Log("in");
                } else if (building.IsNull() && InsideTemple) {
                    if (IsActiveThreadTerminating()) return;
                    InsideTemple = false; Log("out");
                }
            }
            Game->NewScriptFrame();
        }
    }
};
int main()
{
    static const char* const bld[] = { 0, "BUILDING_CHAPEL_GOOD", "BUILDING_CHAPEL_GOOD", "BUILDING_TAVERN", 0, "BUILDING_CHAPEL_GOOD", 0, 0, 0, 0 };
    static const bool alive[] = { true, true, true, true, true, false, true, true, true, true };
    g_log[0] = 0;
    CGameScriptInterface g; g.frame = 0; g.bld = bld; g.alive = alive;
    CV_TempleOfLightScript s; s.Game = &g; s.InsideTemple = false; s.termAfter = 18;
    s.WatchForInsideTemple();
    const char* expect = "frame;in;frame;frame;frame;out;frame;frame;frame;frame;";
    if (std::strcmp(g_log, expect) != 0 || s.InsideTemple) { std::printf("00edbd80_TEST FAIL %s\n", g_log); return 1; }
    std::printf("00edbd80_TEST PASS\n");
    return 0;
}