// Behaviour test for CQ_HeroSoulsBriarScript::PlayCutscene (0x00D823A0): cast HERO and ROSE, run the named cut-scene
// between pause/fix and unfix/unpause, then clear the argument map.
#include <cstdio>
#include <cstring>

static char g_log[256];
static void Log(const char* s) { std::strcat(g_log, s); std::strcat(g_log, ","); }

struct Handle { int id; };
struct CastMap {
    const char* keys[4]; Handle values[4]; int n;
    CastMap() : n(0) {}
    Handle& operator[](const char* key) { keys[n] = key; values[n].id = 0; return values[n++]; }
};
struct Game {
    void StartScriptingEntity(Handle& h) { h.id = 7; Log("script"); }
    void StartMovieSequence(const char* name) { Log(*name ? "movie?" : "movie"); }
    void FixMovieSequenceCamera(bool fix) { Log(fix ? "fix" : "unfix"); }
    void PauseAllNonScriptedEntities(bool pause) { Log(pause ? "pause" : "unpause"); }
};
struct Briar {
    Game* game; Handle rose; int argsCleared; CastMap last;
    void Run(const char* name, CastMap& cast) { Log(name); last = cast; }
    void PlayCutscene(const char* name)
    {
        Handle hero;
        game->StartScriptingEntity(hero);
        CastMap cast;
        cast["HERO"] = hero;
        cast["ROSE"] = rose;
        game->StartMovieSequence("");
        Game* g = game;
        g->PauseAllNonScriptedEntities(true);
        game->FixMovieSequenceCamera(true);
        Run(name, cast);
        game->FixMovieSequenceCamera(false);
        ++argsCleared;
        g->PauseAllNonScriptedEntities(false);
    }
};

int main()
{
    Game game; Briar b; b.game = &game; b.rose.id = 42; b.argsCleared = 0;
    g_log[0] = 0; b.PlayCutscene("CS_BRIAR");
    int ok = std::strcmp(g_log, "script,movie,pause,fix,CS_BRIAR,unfix,unpause,") == 0;
    ok = ok && b.last.n == 2 && std::strcmp(b.last.keys[0], "HERO") == 0 && b.last.values[0].id == 7;
    ok = ok && std::strcmp(b.last.keys[1], "ROSE") == 0 && b.last.values[1].id == 42 && b.argsCleared == 1;
    if (!ok) { std::printf("%s 00d823a0_TEST FAIL\n", g_log); return 1; }
    std::printf("00d823a0_TEST PASS\n");
    return 0;
}