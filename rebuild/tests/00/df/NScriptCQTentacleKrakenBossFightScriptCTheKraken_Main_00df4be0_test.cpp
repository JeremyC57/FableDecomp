#include <cstdio>
#include <cstring>
// behaviour test: CTheKraken::Main -- register the alive filter, run a frame, wait for the kraken's health to reach
// zero, then script the hero, play CS_KRAKEN_OUTRO with the world paused and idle until the script is done
static int g_frames = 0, g_doneAt = 1000, g_filters = 0, g_paused = 0, g_scripted = 0, g_deadAt = 4;
static const char* g_played = 0; static int g_playedFrame = -1;
class CScriptThing { public: int id; };
class CGameScriptInterface { public:
    void NewScriptFrame() const { ++g_frames; }
    float GetHealth(const CScriptThing& t) const { return (t.id == 8 && g_frames >= g_deadAt) ? 0.0f : 100.0f; }
    CScriptThing GetHero() const { CScriptThing t; t.id = 1; return t; }
    void StartScriptingEntity(const CScriptThing& t, int priority) const { if (t.id == 1 && priority == 4) ++g_scripted; }
    void PauseAllNonScriptedEntities(bool p) const { g_paused += p ? 1 : -1; }
};
struct CTheKraken {
    CGameScriptInterface* Game; CScriptThing Me;
    void RegisterFilter() { ++g_filters; }
    bool IsDone() const { return g_frames >= g_doneAt; }
    void RunCutsceneMacro(const char* n) { if (g_paused == 1) { g_played = n; g_playedFrame = g_frames; } }
    void Main();
};
void CTheKraken::Main()
{
    RegisterFilter();
    Game->NewScriptFrame();
    if (IsDone()) return;
    while (!(Game->GetHealth(Me) <= 0.0f)) { Game->NewScriptFrame(); if (IsDone()) return; }
    if (IsDone()) return;
    Game->StartScriptingEntity(Game->GetHero(), 4);
    CGameScriptInterface* game = Game;
    game->PauseAllNonScriptedEntities(true);
    RunCutsceneMacro("CS_KRAKEN_OUTRO");
    game->PauseAllNonScriptedEntities(false);
    for (;;) { Game->NewScriptFrame(); if (IsDone()) return; }
}
int main()
{
    CGameScriptInterface game; CTheKraken k; k.Game = &game; k.Me.id = 8; g_doneAt = 10;
    k.Main();
    bool ok = g_filters == 1 && g_scripted == 1 && g_played && !std::strcmp(g_played, "CS_KRAKEN_OUTRO") && g_playedFrame == 4
           && g_paused == 0 && g_frames == 10;
    g_frames = 0; g_played = 0; g_doneAt = 3;
    k.Main();                                           // done before the kraken dies: no outro
    ok = ok && g_played == 0 && g_filters == 2;
    if (!ok) { std::printf("00df4be0_TEST FAIL\n"); return 1; }
    std::printf("00df4be0_TEST PASS\n");
    return 0;
}