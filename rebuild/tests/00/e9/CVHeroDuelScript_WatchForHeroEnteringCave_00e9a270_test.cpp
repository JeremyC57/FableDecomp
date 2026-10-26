// Behaviour test for CV_HeroDuelScript::WatchForHeroEnteringCave (0x00E9A270): which cutscene plays and the pause pairing.
#include <cstdio>
#include <cstring>
static char g_log[1024];
static void Log(const char* s) { std::strcat(g_log, s); std::strcat(g_log, ";"); }
class CScriptThing { public: const char* name; };
struct CHandle { const char* who; CHandle() : who("") {} bool IsScripting() { return false; } void Release() {} };
struct CCutsceneCastMap {
    CHandle slots[2];
    CHandle& operator[](const char* key) { return slots[std::strcmp(key, "HERO") == 0 ? 1 : 0]; }
};
static void RunCutsceneMacro(const char* name, CCutsceneCastMap& cast, void*, void*, bool, bool) { Log(name); Log(cast["THUNDER"].who); Log(cast["HERO"].who); }
struct CV_HeroDuelScript;
class CGameScriptInterface { public:
    CV_HeroDuelScript* script;
    int frame, heroControlFrame;
    bool levelLoaded;
    CScriptThing hero;
    void NewScriptFrame();
    bool IsLevelLoaded(const char*) { return levelLoaded; }
    CScriptThing GetThingWithScriptName(const char* n) { CScriptThing t; t.name = n; return t; }
    CScriptThing& GetHero() { return hero; }
    bool StartScriptingEntity(const CScriptThing& t, CHandle& h, int) { if (&t == &hero && frame < heroControlFrame) return false; h.who = t.name; return true; }
    void StartMovieSequence(const char*, int&) { Log("movie"); }
    void PauseAllNonScriptedEntities(bool p) { Log(p ? "pause" : "unpause"); }
    void FixMovieSequenceCamera(bool f) { Log(f ? "fix" : "unfix"); }
    void EntitySetCutsceneBehaviour(const CScriptThing& t, long) { Log("behaviour"); }
};
struct CV_HeroDuelScript {
    CGameScriptInterface* Game;
    bool MissionOver, DuelStarted, ThunderEnteredCaveFirst;
    int terminateFrom, checks;
    bool IsActiveThreadTerminating() { return ++checks >= terminateFrom; }
    void WatchForHeroEnteringCave()
    {
        while (!DuelStarted) { Game->NewScriptFrame(); if (IsActiveThreadTerminating()) return; }
        if (IsActiveThreadTerminating()) return;
        while (!Game->IsLevelLoaded("HeroDualAreaCave") && !MissionOver) { Game->NewScriptFrame(); if (IsActiveThreadTerminating()) return; }
        if (IsActiveThreadTerminating()) return;
        if (MissionOver) return;
        if (IsActiveThreadTerminating()) return;
        CScriptThing thunder = Game->GetThingWithScriptName("Thunder");
        CHandle thunderHandle;
        Game->StartScriptingEntity(thunder, thunderHandle, 4);
        CHandle hero;
        if (hero.IsScripting()) hero.Release();
        while (!Game->StartScriptingEntity(Game->GetHero(), hero, 4)) { Game->NewScriptFrame(); if (IsActiveThreadTerminating()) return; }
        if (IsActiveThreadTerminating()) return;
        CCutsceneCastMap cast;
        cast["THUNDER"] = thunderHandle;
        cast["HERO"] = hero;
        int movie = 0;
        Game->StartMovieSequence("", movie);
        CGameScriptInterface* game = Game;
        game->PauseAllNonScriptedEntities(true);
        Game->FixMovieSequenceCamera(true);
        Game->EntitySetCutsceneBehaviour(thunder, 2);
        if (ThunderEnteredCaveFirst) {
            if (IsActiveThreadTerminating()) { game->PauseAllNonScriptedEntities(false); return; }
            RunCutsceneMacro("CAVESTART_1", cast, 0, 0, false, true);
        } else {
            if (IsActiveThreadTerminating()) { game->PauseAllNonScriptedEntities(false); return; }
            RunCutsceneMacro("CAVESTART_2", cast, 0, 0, false, true);
        }
        Game->EntitySetCutsceneBehaviour(thunder, 2);
        Game->FixMovieSequenceCamera(false);
        game->PauseAllNonScriptedEntities(false);
    }
};
void CGameScriptInterface::NewScriptFrame() { ++frame; if (frame == 2) script->DuelStarted = true; if (frame == 4) levelLoaded = true; }
static bool Run(bool first, bool over, int terminateFrom, const char* expect)
{
    g_log[0] = 0;
    CGameScriptInterface game;
    CV_HeroDuelScript s;
    game.script = &s; game.frame = 0; game.heroControlFrame = 6; game.levelLoaded = false; game.hero.name = "Hero";
    s.Game = &game; s.MissionOver = over; s.DuelStarted = false; s.ThunderEnteredCaveFirst = first;
    s.terminateFrom = terminateFrom; s.checks = 0;
    s.WatchForHeroEnteringCave();
    return std::strcmp(g_log, expect) == 0;
}
int main()
{
    bool ok = Run(true, false, 100, "movie;pause;fix;behaviour;CAVESTART_1;Thunder;Hero;behaviour;unfix;unpause;")
        && Run(false, false, 100, "movie;pause;fix;behaviour;CAVESTART_2;Thunder;Hero;behaviour;unfix;unpause;")
        && Run(false, true, 100, "")
        && Run(true, false, 11, "movie;pause;fix;behaviour;unpause;");
    if (!ok) { std::printf("00e9a270_TEST FAIL\n"); return 1; }
    std::printf("00e9a270_TEST PASS\n");
    return 0;
}