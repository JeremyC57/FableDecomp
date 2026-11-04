// Behaviour test for NScript::CV_HeroDuelScript::Duel (0x00E9BE60).
#include <cstdio>
#include <cstring>
static char g_log[4096];
static void Log(const char* s) { std::strcat(g_log, s); std::strcat(g_log, ";"); }
struct C3DVector { float X, Y, Z; };
class CScriptThing { public: int id; C3DVector pos; const C3DVector& GetPos() const { return pos; } };
struct Handle { int who; Handle() : who(0) {} bool IsValid() { return false; } void Release() { Log("release"); } };
struct Cast {
    int hero, thunder;
    Cast() : hero(0), thunder(0) {}
    int& operator[](const char* k) { return !std::strcmp(k, "HERO") ? hero : thunder; }
};
class CGameScriptInterface { public:
    int thunderTries, heroTries; CScriptThing hero; float paused;
    void NewScriptFrame() { Log("frame"); }
    CScriptThing GetThingWithScriptName(const char* n)
    {
        CScriptThing t; t.id = !std::strcmp(n, "Thunder") ? 7 : !std::strcmp(n, "ThunderWaitPos") ? 5 : 0;
        t.pos.X = 10.0f; t.pos.Y = 20.0f; t.pos.Z = 30.0f; return t;
    }
    CScriptThing CreateCreature(const char* def, const C3DVector& pos, const char* name, bool b)
    {
        Log(!std::strcmp(def, "CREATURE_RIVAL_HERO_THUNDER") && pos.X == 10.0f && pos.Z == 30.0f && !std::strcmp(name, "Thunder") && !b ? "create" : "badcreate");
        CScriptThing t; t.id = 7; t.pos = pos; return t;
    }
    CScriptThing& GetHero() { hero.id = 1; return hero; }
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
    void Pause(float s) { paused = s; Log("wait"); }
};
struct CPause {                                   // CWideScreenMagicPauseEntities
    CGameScriptInterface* Game;
    CPause(CGameScriptInterface& g) : Game(&g) { Game->PauseAllNonScriptedEntities(true); }
    ~CPause() { Game->PauseAllNonScriptedEntities(false); }
};
static void RunCutsceneMacro(const char* n, Cast& c, void*, void*, bool a, bool b)
{ Log(c.hero == 1 && c.thunder == 7 && !a && b ? n : "badcutscene"); }
struct CMaster { bool WhisperKilledByHero; };
struct CV_HeroDuelScript {
    CGameScriptInterface* Game; CMaster* MasterScriptData; int termAfter;
    bool IsActiveThreadTerminating() { return --termAfter < 0; }
    void Duel()
    {
        CScriptThing thunder = Game->CreateCreature("CREATURE_RIVAL_HERO_THUNDER", Game->GetThingWithScriptName("ThunderWaitPos").GetPos(), "Thunder", false);
        Handle thunderHandle;
        if (thunderHandle.IsValid()) thunderHandle.Release();
        while (!Game->StartScriptingEntity(thunder, thunderHandle, 4)) { Game->NewScriptFrame(); if (IsActiveThreadTerminating()) return; }
        if (IsActiveThreadTerminating()) return;
        {
            Handle heroHandle;
            if (heroHandle.IsValid()) heroHandle.Release();
            while (!Game->StartScriptingEntity(Game->GetHero(), heroHandle, 4)) { Game->NewScriptFrame(); if (IsActiveThreadTerminating()) return; }
            if (IsActiveThreadTerminating()) return;
            Cast cast;
            cast["HERO"] = heroHandle.who;
            cast["THUNDER"] = thunderHandle.who;
            Game->StartMovieSequence("");
            CPause pause(*Game);
            Game->FixMovieSequenceCamera(true);
            CScriptThing thunderInScene = Game->GetThingWithScriptName("Thunder");
            Game->EntitySetCutsceneBehaviour(thunderInScene, 2);
            if (MasterScriptData->WhisperKilledByHero) {
                if (IsActiveThreadTerminating()) return;
                RunCutsceneMacro("CS_GIBBETHILL_HERODUEL_HILLTOP_A", cast, 0, 0, false, true);
            } else {
                if (IsActiveThreadTerminating()) return;
                RunCutsceneMacro("CS_GIBBETHILL_HERODUEL_HILLTOP_B", cast, 0, 0, false, true);
            }
            Game->EntitySetCutsceneBehaviour(thunderInScene, 1);
            Game->FixMovieSequenceCamera(false);
        }
        Game->Pause(1.0f);
    }
};
static bool Run(int tt, int ht, int term, bool whisper, const char* expect, bool waited)
{
    g_log[0] = 0;
    CGameScriptInterface g; g.thunderTries = tt; g.heroTries = ht; g.paused = 0.0f;
    CMaster m; m.WhisperKilledByHero = whisper;
    CV_HeroDuelScript s; s.Game = &g; s.MasterScriptData = &m; s.termAfter = term;
    s.Duel();
    if (std::strcmp(g_log, expect) != 0) { std::printf("%s\n", g_log); return false; }
    return waited ? (g.paused > 0.999f && g.paused < 1.001f) : g.paused == 0.0f;
}
int main()
{
    bool ok = Run(0, 0, 10, true, "create;movie;pause;fix;cb2;CS_GIBBETHILL_HERODUEL_HILLTOP_A;cb1;unfix;unpause;wait;", true)
        && Run(1, 1, 10, false, "create;frame;frame;movie;pause;fix;cb2;CS_GIBBETHILL_HERODUEL_HILLTOP_B;cb1;unfix;unpause;wait;", true)
        && Run(0, 0, 2, false, "create;movie;pause;fix;cb2;unpause;", false)
        && Run(2, 0, 1, false, "create;frame;frame;", false)
        && Run(0, 0, 0, true, "create;", false);
    if (!ok) { std::printf("00e9be60_TEST FAIL\n"); return 1; }
    std::printf("00e9be60_TEST PASS\n");
    return 0;
}