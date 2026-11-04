// Behaviour test for NScript::CV_HeroDuelScript::FirstFleeCutscene (0x00E9A970).
#include <cstdio>
#include <cstring>
static char g_log[4096];
static void Log(const char* s) { std::strcat(g_log, s); std::strcat(g_log, ";"); }
class CScriptThing { public: int id; };
struct Handle { int who; Handle() : who(0) {} bool IsValid() { return false; } void Release() { Log("release"); } };
struct Cast {
    int hero, thunder;
    Cast() : hero(0), thunder(0) {}
    int& operator[](const char* k) { return !std::strcmp(k, "HERO") ? hero : thunder; }
};
class CGameScriptInterface { public:
    int thunderTries, heroTries; CScriptThing hero;
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
    void EntitySetAsDamageable(const CScriptThing& t, bool d) { Log(t.id != 1 ? "baddmg" : d ? "dmg1" : "dmg0"); }
    void StartMovieSequence(const char* n) { Log(n[0] ? "badmovie" : "movie"); }
    void PauseAllNonScriptedEntities(bool p) { Log(p ? "pause" : "unpause"); }
    void FixMovieSequenceCamera(bool f) { Log(f ? "fix" : "unfix"); }
    void SetTeleportingAsActive(bool a) { Log(a ? "tele1" : "tele0"); }
    void EntitySetAsAllowedToFollowHero(const CScriptThing& t, bool a) { Log(t.id == 7 && a ? "follow" : "badfollow"); }
    void SetEntityAsRegionFollowing(const CScriptThing& leader, const CScriptThing& t, bool a) { Log(leader.id == 1 && t.id == 7 && a ? "region" : "badregion"); }
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
    CGameScriptInterface* Game; CMaster* MasterScriptData; bool OkToShowHealthBar, StopSupportersCheering; int termAfter;
    bool IsActiveThreadTerminating() { return --termAfter < 0; }
    void FirstFleeCutscene()
    {
        OkToShowHealthBar = false;
        StopSupportersCheering = true;
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
            Game->EntitySetAsDamageable(Game->GetHero(), false);
            CScriptThing thunderInScene = Game->GetThingWithScriptName("Thunder");
            Game->EntitySetCutsceneBehaviour(thunderInScene, 2);
            RunCutsceneMacro("CS_GIBBETHILL_HERODUEL_CLIFF", cast, 0, 0, false, true);
            Game->EntitySetCutsceneBehaviour(thunderInScene, 1);
            cast["HERO"] = heroHandle.who;
            cast["THUNDER"] = thunderHandle.who;
            Game->EntitySetCutsceneBehaviour(thunderInScene, 2);
            if (MasterScriptData->WhisperKilledByHero) {
                if (IsActiveThreadTerminating()) return;
                RunCutsceneMacro("CS_GIBBETHILL_HERODUEL_CLIFF_W", cast, 0, 0, false, true);
            } else {
                if (IsActiveThreadTerminating()) return;
                RunCutsceneMacro("CS_GIBBETHILL_HERODUEL_CLIFF_N", cast, 0, 0, false, true);
            }
            Game->EntitySetCutsceneBehaviour(thunderInScene, 1);
            Game->EntitySetAsDamageable(Game->GetHero(), true);
            Game->FixMovieSequenceCamera(false);
        }
        Game->SetTeleportingAsActive(false);
        Game->EntitySetAsAllowedToFollowHero(thunder, true);
        Game->SetEntityAsRegionFollowing(Game->GetHero(), thunder, true);
        OkToShowHealthBar = true;
    }
};
static bool Run(int tt, int ht, int term, bool whisper, const char* expect, bool done)
{
    g_log[0] = 0;
    CGameScriptInterface g; g.thunderTries = tt; g.heroTries = ht;
    CMaster m; m.WhisperKilledByHero = whisper;
    CV_HeroDuelScript s; s.Game = &g; s.MasterScriptData = &m; s.OkToShowHealthBar = true; s.StopSupportersCheering = false; s.termAfter = term;
    s.FirstFleeCutscene();
    if (std::strcmp(g_log, expect) != 0) { std::printf("%s\n", g_log); return false; }
    return s.OkToShowHealthBar == done && s.StopSupportersCheering;
}
int main()
{
    const char* mid = "movie;pause;fix;dmg0;cb2;CS_GIBBETHILL_HERODUEL_CLIFF;cb1;cb2;";
    const char* tail = "cb1;dmg1;unfix;unpause;tele0;follow;region;";
    char w[1024], n[1024], t[1024];
    std::sprintf(w, "frame;%sCS_GIBBETHILL_HERODUEL_CLIFF_W;%s", mid, tail);
    std::sprintf(n, "%sCS_GIBBETHILL_HERODUEL_CLIFF_N;%s", mid, tail);
    std::sprintf(t, "%sunpause;", mid);
    bool ok = Run(1, 0, 10, true, w, true)
        && Run(0, 0, 10, false, n, true)
        && Run(0, 0, 2, false, t, false)
        && Run(3, 0, 1, false, "frame;frame;", false)
        && Run(0, 2, 2, false, "frame;frame;", false);
    if (!ok) { std::printf("00e9a970_TEST FAIL\n"); return 1; }
    std::printf("00e9a970_TEST PASS\n");
    return 0;
}