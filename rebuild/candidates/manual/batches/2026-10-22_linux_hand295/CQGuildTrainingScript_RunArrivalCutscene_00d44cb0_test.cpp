#include <cstdio>
#include <cstring>
// behaviour test: CQ_GuildTrainingScript::RunArrivalCutscene -- evening, create the rival hero at the maze marker,
// script him and the hero (retrying each frame), cast them as MAZE / HERO, play CS_GUILD_ARRIVE with the world paused
// and the camera fixed, fade out, then remove the rival
static int g_frames = 0, g_endAt = 1000, g_scriptOkAt = 2, g_paused = 0, g_removed = 0, g_cast = 0;
static float g_time = 0; static const char* g_played = 0;
struct CScriptThing { int id; };
struct CGameScriptInterface {
    void NewScriptFrame() const { ++g_frames; }
    void SetTimeOfDay(float t) const { g_time = t; }
    CScriptThing CreateCreature(const char* def) const { CScriptThing t; t.id = std::strcmp(def, "CREATURE_RIVAL_HERO_MAZE_CUTSCENE") == 0 ? 5 : 0; return t; }
    bool StartScriptingEntity(const CScriptThing&, int priority) const { return priority == 4 && g_frames >= g_scriptOkAt; }
    void PauseAllNonScriptedEntities(bool p) const { g_paused += p ? 1 : -1; }
    void RemoveThing(const CScriptThing& t) const { if (t.id == 5) ++g_removed; }
};
struct CQ_GuildTrainingScript {
    CGameScriptInterface* Game;
    bool IsActiveThreadTerminating() const { return g_frames >= g_endAt; }
    void RunCutsceneMacro(const char* name, int cast) { if (g_paused == 1) g_played = name; g_cast = cast; }
    void RunArrivalCutscene();
};
void CQ_GuildTrainingScript::RunArrivalCutscene()
{
    Game->SetTimeOfDay(19.0f);
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating()) return;
    CScriptThing rival = Game->CreateCreature("CREATURE_RIVAL_HERO_MAZE_CUTSCENE");
    {
        CScriptThing hero; hero.id = 1;
        while (!Game->StartScriptingEntity(rival, 4)) { Game->NewScriptFrame(); if (IsActiveThreadTerminating()) return; }
        while (!Game->StartScriptingEntity(hero, 4)) { Game->NewScriptFrame(); if (IsActiveThreadTerminating()) return; }
        int cast = 2;                                   // "MAZE" and "HERO"
        Game->PauseAllNonScriptedEntities(true);
        RunCutsceneMacro("CS_GUILD_ARRIVE", cast);
        Game->PauseAllNonScriptedEntities(false);
    }
    Game->RemoveThing(rival);
}
int main()
{
    CGameScriptInterface game; CQ_GuildTrainingScript s; s.Game = &game;
    s.RunArrivalCutscene();
    bool ok = g_time == 19.0f && g_played && std::strcmp(g_played, "CS_GUILD_ARRIVE") == 0 && g_cast == 2
           && g_paused == 0 && g_removed == 1;
    g_frames = 0; g_played = 0; g_removed = 0; g_scriptOkAt = 50; g_endAt = 3;
    s.RunArrivalCutscene();                             // scripting never succeeds: no cutscene, rival left in place
    ok = ok && g_played == 0 && g_removed == 0;
    if (!ok) { std::printf("00d44cb0_TEST FAIL\n"); return 1; }
    std::printf("00d44cb0_TEST PASS\n");
    return 0;
}
