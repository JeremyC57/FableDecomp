#include <cstdio>
#include <cstring>
// behaviour test: CQ_GuildTrainingScript::KeepBookcaseExitRemoved -- until the training reaches state 9, every time
// the hero leaves and re-enters HeroGuildComplex the secret bookcase's exit point is switched off again
static int g_frames = 0, g_endAt = 1000, g_disabled = 0, g_stateAt = 100;
static const char g_loaded[] = "110011100110000000000";
struct CScriptThing { int id; };
struct CGameScriptInterface {
    void NewScriptFrame() const { ++g_frames; }
    bool IsLevelLoaded(const char* n) const { return std::strcmp(n, "HeroGuildComplex") == 0 && g_loaded[g_frames] == '1'; }
    CScriptThing GetThingWithScriptName(const char* n) const { CScriptThing t; t.id = std::strcmp(n, "SecretBookcase") == 0 ? 3 : 0; return t; }
    CScriptThing GetNearestWithDefName(const CScriptThing& c, const char* d) const
    { CScriptThing t; t.id = (c.id == 3 && std::strcmp(d, "REGION_EXIT_POINT") == 0) ? 9 : 0; return t; }
    void SetRegionExitAsActive(const CScriptThing& t, bool on) const { if (t.id == 9 && !on) ++g_disabled; }
};
struct CQ_GuildTrainingScript {
    CGameScriptInterface* Game; long GameState;
    bool IsActiveThreadTerminating() const { return g_frames >= g_endAt; }
    void KeepBookcaseExitRemoved();
};
void CQ_GuildTrainingScript::KeepBookcaseExitRemoved()
{
    CScriptThing bookcase = Game->GetThingWithScriptName("SecretBookcase");
    CScriptThing exitPoint = Game->GetNearestWithDefName(bookcase, "REGION_EXIT_POINT");
    while (GameState != 9) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return;
        while (Game->IsLevelLoaded("HeroGuildComplex")) { Game->NewScriptFrame(); if (IsActiveThreadTerminating()) return; }
        while (!Game->IsLevelLoaded("HeroGuildComplex")) { Game->NewScriptFrame(); if (IsActiveThreadTerminating()) return; }
        if (g_frames >= g_stateAt) GameState = 9;
        if (GameState != 9) Game->SetRegionExitAsActive(exitPoint, false);
    }
}
int main()
{
    CGameScriptInterface game; CQ_GuildTrainingScript s; s.Game = &game; s.GameState = 2; g_stateAt = 9;
    s.KeepBookcaseExitRemoved();                        // re-entries at frames 4 and 9: the second one sees state 9
    bool ok = g_disabled == 1 && s.GameState == 9;
    g_frames = 0; g_disabled = 0; s.GameState = 9;
    s.KeepBookcaseExitRemoved();                        // already finished: nothing happens
    ok = ok && g_disabled == 0 && g_frames == 0;
    if (!ok) { std::printf("00d3c8f0_TEST FAIL\n"); return 1; }
    std::printf("00d3c8f0_TEST PASS\n");
    return 0;
}
