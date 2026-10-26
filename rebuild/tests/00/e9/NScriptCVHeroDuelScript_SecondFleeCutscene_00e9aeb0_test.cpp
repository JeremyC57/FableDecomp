// Behaviour test for CV_HeroDuelScript::SecondFleeCutscene (0x00E9AEB0).
#include <cstdio>
#include <cstring>
static char g_log[2048];
static void Log(const char* s) { std::strcat(g_log, s); std::strcat(g_log, ";"); }
class CScriptThing { public: int id; };
struct Handle { int who; bool IsScripting() { return false; } void Release() { Log("release"); } };
struct Cast {
    int hero, thunder;
    int& operator[](const char* k) { return !std::strcmp(k, "HERO") ? hero : thunder; }
};
class CGameScriptInterface { public:
    int thunderTries, heroTries; CScriptThing hero;
    void NewScriptFrame() { Log("frame"); }
    CScriptThing GetThingWithScriptName(const char* n) { CScriptThing t; t.id = !std::strcmp(n, "Thunder") ? 7 : 0; return t; }
    CScriptThing& GetHero() { hero.id = 1; return hero; }
    void MiniMapRemoveMarker(const CScriptThing& t) { Log(t.id == 7 ? "unmark" : "badunmark"); }
    bool StartScriptingEntity(const CScriptThing& t, Handle& h, long pri)
    {
        if (pri != 4) Log("badpri");
        h.who = t.id;
        return t.id == 7 ? --thunderTries < 0 : --heroTries < 0;
    }
    void EntitySetCutsceneBehaviour(const CScriptThing& t, long b) { Log(t.id != 7 ? "badcb" : b == 2 ? "cb2" : b == 1 ? "cb1" : "badcb"); }
    void StartMovieSequence(const char* n) { Log(n[0] ? "badmovie" : "movie"); }
    void PauseAllNonScriptedEntities(bool p) { Log(p ? "pause" : "unpause"); }
    void FixMovieSequenceCamera(bool f) { Log(f ? "fix" : "unfix"); }
};
static void RunCutsceneMacro(const char* n, Cast& c, void*, void*, bool a, bool b)
{ Log(!std::strcmp(n, "CS_GIBBETHILL_HERODUEL_FLEE") && c.hero == 1 && c.thunder == 7 && !a && b ? "cutscene" : "badcutscene"); }
struct CV_HeroDuelScript {
    CGameScriptInterface* Game; bool OkToShowHealthBar, ThunderEnteredCaveFirst; int termAfter;
    bool IsActiveThreadTerminating() { return --termAfter < 0; }
    void SecondFleeCutscene()
    {
        OkToShowHealthBar = false;
        CScriptThing thunder = Game->GetThingWithScriptName("Thunder");
        Game->MiniMapRemoveMarker(thunder);
        {
            Handle thunderHandle;
            if (thunderHandle.IsScripting()) thunderHandle.Release();
            while (!Game->StartScriptingEntity(thunder, thunderHandle, 4)) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating()) return;
            }
            if (IsActiveThreadTerminating()) return;
            Game->EntitySetCutsceneBehaviour(thunder, 2);
            Handle heroHandle;
            if (heroHandle.IsScripting()) heroHandle.Release();
            while (!Game->StartScriptingEntity(Game->GetHero(), heroHandle, 4)) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating()) return;
            }
            if (IsActiveThreadTerminating()) return;
            {
                Cast cast;
                cast["HERO"] = heroHandle.who;
                cast["THUNDER"] = thunderHandle.who;
                Game->StartMovieSequence("");
                CGameScriptInterface* game = Game;
                game->PauseAllNonScriptedEntities(true);
                Game->FixMovieSequenceCamera(true);
                RunCutsceneMacro("CS_GIBBETHILL_HERODUEL_FLEE", cast, 0, 0, false, true);
                Game->FixMovieSequenceCamera(false);
                game->PauseAllNonScriptedEntities(false);
            }
        }
        Game->EntitySetCutsceneBehaviour(thunder, 1);
        ThunderEnteredCaveFirst = true;
        OkToShowHealthBar = true;
    }
};
static bool Run(int tt, int ht, int term, const char* expect, bool done)
{
    g_log[0] = 0;
    CGameScriptInterface g; g.thunderTries = tt; g.heroTries = ht;
    CV_HeroDuelScript s; s.Game = &g; s.OkToShowHealthBar = true; s.ThunderEnteredCaveFirst = false; s.termAfter = term;
    s.SecondFleeCutscene();
    if (std::strcmp(g_log, expect) != 0) { std::printf("%s\n", g_log); return false; }
    return s.ThunderEnteredCaveFirst == done && s.OkToShowHealthBar == done;
}
int main()
{
    bool ok = Run(1, 2, 10, "unmark;frame;cb2;frame;frame;movie;pause;fix;cutscene;unfix;unpause;cb1;", true)
        && Run(3, 0, 1, "unmark;frame;frame;", false)
        && Run(0, 0, 0, "unmark;", false);
    if (!ok) { std::printf("00e9aeb0_TEST FAIL\n"); return 1; }
    std::printf("00e9aeb0_TEST PASS\n");
    return 0;
}