#include <cstdio>
#include <cstring>
// behaviour test: CQ_CinemaTestScript::ArenaIntroduction -- script the hero at priority 4, cast him as "Hero" and
// play the ARENA_HOH_INTRO cutscene macro (no frame waits, no pause)
static int g_scripted = 0; static const char* g_played = 0; static const char* g_castName = 0;
class CScriptThing { public: int id; };
class CGameScriptInterface { public:
    CScriptThing GetHero() const { CScriptThing t; t.id = 1; return t; }
    void StartScriptingEntity(const CScriptThing& t, int priority) const { if (t.id == 1 && priority == 4) ++g_scripted; }
};
struct CQ_CinemaTestScript {
    CGameScriptInterface* Game;
    void RunCutsceneMacro(const char* n, const char* cast) { g_played = n; g_castName = cast; }
    void ArenaIntroduction();
};
void CQ_CinemaTestScript::ArenaIntroduction()
{
    Game->StartScriptingEntity(Game->GetHero(), 4);
    RunCutsceneMacro("ARENA_HOH_INTRO", "Hero");
}
int main()
{
    CGameScriptInterface game; CQ_CinemaTestScript s; s.Game = &game;
    s.ArenaIntroduction();
    bool ok = g_scripted == 1 && g_played && !std::strcmp(g_played, "ARENA_HOH_INTRO") && !std::strcmp(g_castName, "Hero");
    if (!ok) { std::printf("00f02210_TEST FAIL\n"); return 1; }
    std::printf("00f02210_TEST PASS\n");
    return 0;
}