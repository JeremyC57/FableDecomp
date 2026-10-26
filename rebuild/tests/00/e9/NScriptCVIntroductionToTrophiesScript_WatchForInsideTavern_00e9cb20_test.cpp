// Behaviour test for CV_IntroductionToTrophiesScript::WatchForInsideTavern (0x00E9CB20).
#include <cstdio>
#include <cstring>
static char g_log[1024];
static void Log(const char* s) { std::strcat(g_log, s); std::strcat(g_log, ";"); }
class CScriptThing { public:
    const char* def; bool alive;
    bool IsNull() const { return def == 0; }
    bool IsAlive() const { return alive; }
    const char* GetDefName() const { return def ? def : ""; }
};
class CGameScriptInterface { public:
    int frame; const char* const* bld; bool briarAlive; CScriptThing hero;
    CScriptThing& GetHero() { hero.alive = true; hero.def = "HERO"; return hero; }
    CScriptThing EntityGetStandingInsideBuilding(const CScriptThing&) { CScriptThing t; t.def = bld[frame]; t.alive = true; return t; }
    CScriptThing GetThingWithScriptName(const char* n) { CScriptThing t; t.def = n; t.alive = std::strcmp(n, "BriarRose") == 0 && briarAlive; return t; }
    CScriptThing GetRandomThingWithScriptName(const char* n) { CScriptThing t; t.def = n; t.alive = std::strcmp(n, "TrophiesIntroWatcher") == 0; return t; }
    void EntitySetFacingAngleTowardsThing(const CScriptThing& a, const CScriptThing& b, bool f)
    { Log(!std::strcmp(a.def, "BriarRose") && !std::strcmp(b.def, "TrophiesIntroWatcher") && !f ? "face" : "badface"); }
    void NewScriptFrame() { Log("frame"); ++frame; }
};
struct CV_IntroductionToTrophiesScript {
    CGameScriptInterface* Game; bool InsideTavern; int termAfter;
    bool IsActiveThreadTerminating() { return --termAfter < 0; }
    void WatchForInsideTavern()
    {
        CScriptThing building; building.def = 0;
        for (;;) {
            if (IsActiveThreadTerminating()) return;
            if (Game->GetHero().IsAlive()) {
                if (IsActiveThreadTerminating()) return;
                building = Game->EntityGetStandingInsideBuilding(Game->GetHero());
                if (!InsideTavern && !building.IsNull() && std::strcmp(building.GetDefName(), "BUILDING_BS_SLUM_INN_01") == 0) {
                    if (IsActiveThreadTerminating()) return;
                    InsideTavern = true; Log("in");
                } else if (InsideTavern && building.IsNull()) {
                    if (IsActiveThreadTerminating()) return;
                    InsideTavern = false; Log("out");
                    CScriptThing briar = Game->GetThingWithScriptName("BriarRose");
                    CScriptThing watcher = Game->GetRandomThingWithScriptName("TrophiesIntroWatcher");
                    if (briar.IsAlive() && watcher.IsAlive()) {
                        if (IsActiveThreadTerminating()) return;
                        Game->NewScriptFrame();
                        if (IsActiveThreadTerminating()) return;
                        Game->NewScriptFrame();
                        if (IsActiveThreadTerminating()) return;
                        Game->EntitySetFacingAngleTowardsThing(briar, watcher, false);
                        Game->NewScriptFrame();
                        if (IsActiveThreadTerminating()) return;
                        Game->NewScriptFrame();
                        if (IsActiveThreadTerminating()) return;
                    }
                }
            }
            Game->NewScriptFrame();
        }
    }
};
static bool Run(bool briarAlive, const char* expect)
{
    static const char* const bld[] = { "BUILDING_TAVERN", "BUILDING_BS_SLUM_INN_01", "BUILDING_BS_SLUM_INN_01", 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    g_log[0] = 0;
    CGameScriptInterface g; g.frame = 0; g.bld = bld; g.briarAlive = briarAlive;
    CV_IntroductionToTrophiesScript s; s.Game = &g; s.InsideTavern = false; s.termAfter = 20;
    s.WatchForInsideTavern();
    if (std::strcmp(g_log, expect) != 0) { std::printf("%s\n", g_log); return false; }
    return !s.InsideTavern;
}
int main()
{
    bool ok = Run(true, "frame;in;frame;frame;out;frame;frame;face;frame;frame;frame;frame;frame;")
        && Run(false, "frame;in;frame;frame;out;frame;frame;frame;frame;frame;frame;");
    if (!ok) { std::printf("00e9cb20_TEST FAIL\n"); return 1; }
    std::printf("00e9cb20_TEST PASS\n");
    return 0;
}