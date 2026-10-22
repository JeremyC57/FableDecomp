#include <cstdio>
#include <cstring>
static char g_log[4096];
static int g_frame = 0;
static int g_termAt = 1000;
static void Log(const char* s) { char b[32]; std::sprintf(b, "@%d;", g_frame); std::strcat(g_log, s); std::strcat(g_log, b); }
static void Reset(int termAt) { g_log[0] = 0; g_frame = 0; g_termAt = termAt; }
static int Fail(const char* addr, const char* what) { std::printf("%s_TEST FAIL %s [%s]\n", addr, what, g_log); return 1; }
#define CHECK(addr, cond) do { if (!(cond)) return Fail(addr, #cond); } while (0)
// behaviour test: CCS_PlayCutsceneScript::Main waits for the console request, plays the named cut-scene with the hero cast twice, clears the request
static int g_requestAt = -1, g_alive = 0;
static bool GlobalConsolePlayCutscene = false;
static char GlobalConsoleCutsceneName[32] = "CS_TEST";
struct CScriptThing { int Id; };
struct CHandle {
    int Who;
    CHandle() : Who(0) { ++g_alive; }
    ~CHandle() { --g_alive; }
    void Release() { Log("release"); }
};
struct CCast {
    int n; char keys[4][16]; int who[4];
    CCast() : n(0) { ++g_alive; }
    ~CCast() { --g_alive; }
    int& operator[](const char* k) { std::strcpy(keys[n], k); return who[n++]; }
};
struct CMovie { CMovie() { ++g_alive; } ~CMovie() { --g_alive; } };
struct CGame {
    CScriptThing Hero;
    void NewScriptFrame() const { ++g_frame; if (g_frame == g_requestAt) GlobalConsolePlayCutscene = true; }
    bool StartScriptingEntity(const CScriptThing& t, CHandle& h, int p) { char x[32]; std::sprintf(x, "start %d %d", t.Id, p); Log(x); h.Who = t.Id; return true; }
    CScriptThing& GetHero() { return Hero; }
    void StartMovieSequence(const char* n, CMovie&) { char x[32]; std::sprintf(x, "movie[%s]", n); Log(x); }
    void PauseAllNonScriptedEntities(bool b) { Log(b ? "pause1" : "pause0"); }
};
static void RunCutscene(const char* name, CCast& c, bool a5, bool a6)
{
    char x[128]; std::sprintf(x, "run %s %s=%d %s=%d %d%d", name, c.keys[0], c.who[0], c.keys[1], c.who[1], (int)a5, (int)a6); Log(x);
}
struct CCS_PlayCutsceneScript {
    CGame* Game;
    bool IsActiveThreadTerminating() const { return g_frame >= g_termAt; }
    void Main();
};
void CCS_PlayCutsceneScript::Main()
{
    while (!IsActiveThreadTerminating()) {
        while (!GlobalConsolePlayCutscene) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
        }
        if (IsActiveThreadTerminating())
            return;
        {
            CCast cast;
            CHandle hero;
            Game->StartScriptingEntity(Game->GetHero(), hero, 4);
            cast["Hero"] = hero.Who;
            cast["HERO"] = hero.Who;
            {
                CMovie movie;
                Game->StartMovieSequence("", movie);
                CGame* game = Game;
                game->PauseAllNonScriptedEntities(true);
                RunCutscene(GlobalConsoleCutsceneName, cast, true, true);
                game->PauseAllNonScriptedEntities(false);
            }
            hero.Release();
            GlobalConsolePlayCutscene = false;
        }
        Game->NewScriptFrame();
    }
}
int main()
{
    CGame game; game.Hero.Id = 1;
    CCS_PlayCutsceneScript s; s.Game = &game;
    Reset(6); g_requestAt = 3;
    s.Main();
    CHECK("00f01480", std::strcmp(g_log, "start 1 4@3;movie[]@3;pause1@3;run CS_TEST Hero=1 HERO=1 11@3;pause0@3;release@3;") == 0);
    CHECK("00f01480", !GlobalConsolePlayCutscene && g_frame == 6 && g_alive == 0);
    Reset(0); g_requestAt = -1;
    s.Main();
    CHECK("00f01480", g_log[0] == 0 && g_frame == 0);
    Reset(2); GlobalConsolePlayCutscene = true;
    s.Main();
    CHECK("00f01480", std::strcmp(g_log, "start 1 4@0;movie[]@0;pause1@0;run CS_TEST Hero=1 HERO=1 11@0;pause0@0;release@0;") == 0 && g_frame == 2 && g_alive == 0);
    std::printf("00f01480_TEST PASS\n");
    return 0;
}
