// Behaviour test for NScript::CV_HeroDuelScript::DuelLost (0x00E9B240).
#include <cstdio>
#include <cstring>
#include <cmath>
static char g_log[4096];
static void Log(const char* s) { std::strcat(g_log, s); std::strcat(g_log, ";"); }
class CScriptThing { public: int id; };
struct Handle { int who; Handle() : who(0) {} bool IsValid() { return false; } void Release() { Log("release"); } };
struct Cast {
    int hero, thunder;
    Cast() : hero(0), thunder(0) {}
    int& operator[](const char* k) { return !std::strcmp(k, "HERO") ? hero : thunder; }
};
struct CScriptDef { float HeroDuelDefeatThunderMorality; int HeroDuelDefeatThunderXP; };
static CScriptDef g_def = { 2.5f, 300 };
static CScriptDef* pGlobalScriptDef = &g_def;
class CGameScriptInterface { public:
    int thunderTries, heroTries; CScriptThing hero; int xp; float morality;
    void NewScriptFrame() { Log("frame"); }
    CScriptThing GetThingWithScriptName(const char* n) { CScriptThing t; t.id = !std::strcmp(n, "Thunder") ? 7 : 0; return t; }
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
    void GiveHeroExperience(long x) { xp = x; Log("xp"); }
    void GiveHeroMorality(float m) { morality = m; Log("morality"); }
    void GiveHeroObject(const char* o, long n, bool b) { Log(!std::strcmp(o, "OBJECT_TROPHY_THUNDER") && n == -1 && !b ? "trophy" : "badobject"); }
    void SetEntityAsRegionFollowing(const CScriptThing& leader, const CScriptThing& t, bool a) { Log(leader.id == 1 && t.id == 7 && !a ? "unfollow" : "badregion"); }
};
struct CPause {                                   // CWideScreenMagicPauseEntities
    CGameScriptInterface* Game;
    CPause(CGameScriptInterface& g) : Game(&g) { Game->PauseAllNonScriptedEntities(true); }
    ~CPause() { Game->PauseAllNonScriptedEntities(false); }
};
static void RunCutsceneMacro(const char* n, Cast& c, void*, void*, bool a, bool b)
{ Log(c.hero == 1 && c.thunder == 7 && !a && b ? n : "badcutscene"); }
struct CMaster { bool DefeatedThunder, WhisperKilledByHero; };
struct CV_HeroDuelScript {
    CGameScriptInterface* Game; CMaster* MasterScriptData; bool MissionSucceeded, MissionOver, ThunderDefeated; int termAfter;
    bool IsActiveThreadTerminating() { return --termAfter < 0; }
    void DuelLost()
    {
        ThunderDefeated = true;
        CScriptThing thunder = Game->GetThingWithScriptName("Thunder");
        {
            Handle thunderHandle;
            if (thunderHandle.IsValid()) thunderHandle.Release();
            while (!Game->StartScriptingEntity(thunder, thunderHandle, 4)) { Game->NewScriptFrame(); if (IsActiveThreadTerminating()) return; }
            if (IsActiveThreadTerminating()) return;
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
            Game->EntitySetCutsceneBehaviour(thunder, 2);
            if (MasterScriptData->WhisperKilledByHero) {
                if (IsActiveThreadTerminating()) return;
                RunCutsceneMacro("CS_GIBBETHILL_HERODUEL_END_W", cast, 0, 0, false, true);
            } else {
                if (IsActiveThreadTerminating()) return;
                RunCutsceneMacro("CS_GIBBETHILL_HERODUEL_END", cast, 0, 0, false, true);
            }
            Game->EntitySetCutsceneBehaviour(thunder, 1);
            Game->FixMovieSequenceCamera(false);
        }
        Game->GiveHeroExperience(pGlobalScriptDef->HeroDuelDefeatThunderXP);
        Game->GiveHeroMorality(pGlobalScriptDef->HeroDuelDefeatThunderMorality);
        Game->GiveHeroObject("OBJECT_TROPHY_THUNDER", -1, false);
        MasterScriptData->DefeatedThunder = true;
        Game->SetEntityAsRegionFollowing(Game->GetHero(), thunder, false);
        MissionSucceeded = true;
        MissionOver = true;
    }
};
static bool Run(int tt, int ht, int term, bool whisper, const char* expect, bool done)
{
    g_log[0] = 0;
    CGameScriptInterface g; g.thunderTries = tt; g.heroTries = ht; g.xp = 0; g.morality = 0.0f;
    CMaster m; m.DefeatedThunder = false; m.WhisperKilledByHero = whisper;
    CV_HeroDuelScript s; s.Game = &g; s.MasterScriptData = &m; s.MissionSucceeded = s.MissionOver = s.ThunderDefeated = false; s.termAfter = term;
    s.DuelLost();
    if (std::strcmp(g_log, expect) != 0) { std::printf("%s\n", g_log); return false; }
    if (!s.ThunderDefeated) return false;
    if (done) return s.MissionSucceeded && s.MissionOver && m.DefeatedThunder && g.xp == 300 && std::fabs(g.morality - 2.5f) < 1e-6f;
    return !s.MissionSucceeded && !s.MissionOver && !m.DefeatedThunder && g.xp == 0;
}
int main()
{
    const char* tail = "cb1;unfix;unpause;xp;morality;trophy;unfollow;";
    char w[1024], n[1024];
    std::sprintf(w, "frame;movie;pause;fix;cb2;CS_GIBBETHILL_HERODUEL_END_W;%s", tail);
    std::sprintf(n, "movie;pause;fix;cb2;CS_GIBBETHILL_HERODUEL_END;%s", tail);
    bool ok = Run(0, 1, 10, true, w, true)
        && Run(0, 0, 10, false, n, true)
        && Run(0, 0, 2, true, "movie;pause;fix;cb2;unpause;", false)
        && Run(2, 0, 1, false, "frame;frame;", false)
        && Run(0, 0, 0, false, "", false);
    if (!ok) { std::printf("00e9b240_TEST FAIL\n"); return 1; }
    std::printf("00e9b240_TEST PASS\n");
    return 0;
}