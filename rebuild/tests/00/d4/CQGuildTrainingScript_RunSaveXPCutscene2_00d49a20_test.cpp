#include <cstdio>
#include <cstring>
// behaviour test: CQ_GuildTrainingScript::RunSaveXPCutscene2 -- script the real guildmaster and then the hero at priority 4
// (retrying each frame), cast them as GM / HERO and play CS_GUILD_SAVEXP2 with the world paused and the camera fixed
static int g_frames = 0, g_endAt = 1000, g_gmOkAt = 1, g_heroOkAt = 3, g_paused = 0, g_fixed = 0, g_cast = 0;
static const char* g_played = 0;
struct CScriptThing { int id; };
struct CGameScriptInterface {
    void NewScriptFrame() const { ++g_frames; }
    CScriptThing GetThingWithScriptName(const char* n) const { CScriptThing t; t.id = std::strcmp(n, "TheRealGuildmaster") == 0 ? 2 : 0; return t; }
    CScriptThing GetHero() const { CScriptThing t; t.id = 1; return t; }
    bool StartScriptingEntity(const CScriptThing& t, int priority) const
    { return priority == 4 && ((t.id == 2 && g_frames >= g_gmOkAt) || (t.id == 1 && g_frames >= g_heroOkAt)); }
    void PauseAllNonScriptedEntities(bool p) const { g_paused += p ? 1 : -1; }
    void FixMovieSequenceCamera(bool f) const { g_fixed += f ? 1 : -1; }
};
struct CQ_GuildTrainingScript {
    CGameScriptInterface* Game;
    bool IsActiveThreadTerminating() const { return g_frames >= g_endAt; }
    void RunCutsceneMacro(const char* name, int cast) { if (g_paused == 1 && g_fixed == 1) g_played = name; g_cast = cast; }
    void RunSaveXPCutscene2();
};
void CQ_GuildTrainingScript::RunSaveXPCutscene2()
{
    CScriptThing guildmaster = Game->GetThingWithScriptName("TheRealGuildmaster");
    while (!Game->StartScriptingEntity(guildmaster, 4)) { Game->NewScriptFrame(); if (IsActiveThreadTerminating()) return; }
    if (IsActiveThreadTerminating()) return;
    while (!Game->StartScriptingEntity(Game->GetHero(), 4)) { Game->NewScriptFrame(); if (IsActiveThreadTerminating()) return; }
    if (IsActiveThreadTerminating()) return;
    int cast = 2;                                       // "GM" and "HERO"
    CGameScriptInterface* game = Game;
    game->PauseAllNonScriptedEntities(true);
    Game->FixMovieSequenceCamera(true);
    RunCutsceneMacro("CS_GUILD_SAVEXP2", cast);
    Game->FixMovieSequenceCamera(false);
    game->PauseAllNonScriptedEntities(false);
}
int main()
{
    CGameScriptInterface game; CQ_GuildTrainingScript s; s.Game = &game;
    s.RunSaveXPCutscene2();
    bool ok = g_played && std::strcmp(g_played, "CS_GUILD_SAVEXP2") == 0 && g_cast == 2 && g_paused == 0 && g_fixed == 0 && g_frames == 3;
    g_frames = 0; g_played = 0; g_heroOkAt = 99; g_endAt = 5;
    s.RunSaveXPCutscene2();                                             // the hero never becomes scriptable: no cutscene
    ok = ok && g_played == 0 && g_paused == 0;
    if (!ok) { std::printf("00d49a20_TEST FAIL\n"); return 1; }
    std::printf("00d49a20_TEST PASS\n");
    return 0;
}