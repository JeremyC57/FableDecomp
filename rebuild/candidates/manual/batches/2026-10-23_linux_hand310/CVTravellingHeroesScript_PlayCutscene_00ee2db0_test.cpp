// Behaviour test for CV_TravellingHeroesScript::PlayCutscene (0x00EE2DB0).
#include <cstdio>
#include <cstring>
#include <map>
#include <string>
static char g_log[1024];
static void Log(const char* s) { std::strcat(g_log, s); std::strcat(g_log, ";"); }
struct CScriptThing { int id; };
struct Handle { int who; };
typedef std::map<std::string, int> Cast;
struct ArgMap { int n; void clear() { n = 0; Log("clearargs"); } };
struct CGameScriptInterface {
    CScriptThing hero;
    CScriptThing& GetHero() { hero.id = 1; return hero; }
    bool StartScriptingEntity(const CScriptThing& t, Handle& h, long pri) { h.who = pri == 4 ? t.id : -1; return true; }
    void StartMovieSequence(const char* n) { Log(n[0] ? "badmovie" : "movie"); }
    void PauseAllNonScriptedEntities(bool p) { Log(p ? "pause" : "unpause"); }
    void FixMovieSequenceCamera(bool f) { Log(f ? "fix" : "unfix"); }
};
static void RunCutsceneMacro(const std::string& n, Cast& c, void* flags, ArgMap* args, bool a, bool b)
{
    bool ok = n == "CS_TEST" && c["HERO"] == 1 && c["RIVAL"] == 11 && c["GIRL"] == 12 && c["BOY"] == 13 && !flags && args && args->n == 5 && !a && b;
    Log(ok ? "cutscene" : "badcutscene");
}
struct CV_TravellingHeroesScript {
    CGameScriptInterface* Game; Handle seh_Rival, seh_Girl, seh_Boy; ArgMap csargs;
    void PlayCutscene(std::string name)
    {
        Handle hero;
        Game->StartScriptingEntity(Game->GetHero(), hero, 4);
        Cast cast;
        cast["HERO"] = hero.who;
        cast["RIVAL"] = seh_Rival.who;
        cast["GIRL"] = seh_Girl.who;
        cast["BOY"] = seh_Boy.who;
        Game->StartMovieSequence("");
        CGameScriptInterface* game = Game;
        game->PauseAllNonScriptedEntities(true);
        Game->FixMovieSequenceCamera(true);
        RunCutsceneMacro(name, cast, 0, &csargs, false, true);
        Game->FixMovieSequenceCamera(false);
        csargs.clear();
        game->PauseAllNonScriptedEntities(false);
    }
};
int main()
{
    g_log[0] = 0;
    CGameScriptInterface g;
    CV_TravellingHeroesScript s; s.Game = &g; s.seh_Rival.who = 11; s.seh_Girl.who = 12; s.seh_Boy.who = 13; s.csargs.n = 5;
    s.PlayCutscene("CS_TEST");
    if (std::strcmp(g_log, "movie;pause;fix;cutscene;unfix;clearargs;unpause;") != 0 || s.csargs.n != 0) { std::printf("00ee2db0_TEST FAIL %s\n", g_log); return 1; }
    std::printf("00ee2db0_TEST PASS\n");
    return 0;
}
