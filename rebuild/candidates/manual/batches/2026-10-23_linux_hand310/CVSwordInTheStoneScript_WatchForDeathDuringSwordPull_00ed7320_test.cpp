// Behaviour test for CV_SwordInTheStoneScript::WatchForDeathDuringSwordPull (0x00ED7320).
#include <cstdio>
#include <cstring>
static char g_log[1024];
static void Log(const char* s) { std::strcat(g_log, s); std::strcat(g_log, ";"); }
struct CScriptThing {
    int frame; const int* ev;
    bool MsgIsKilledBy(const char* who) const { return who[0] == 0 && ev[frame] == 1; }
    bool MsgIsHitBy(const char* who) const { return std::strcmp(who, "SCRIPT_NAME_HERO") == 0 && ev[frame] == 2; }
    bool MsgIsHitByAnyAggressiveSpecialAbilityFrom(const char* who) const { return std::strcmp(who, "SCRIPT_NAME_HERO") == 0 && ev[frame] == 3; }
};
struct Handle { void ClearCommands() { Log("clear"); } };
struct CGameScriptInterface {
    CScriptThing* puller;
    void NewScriptFrame() { Log("frame"); ++puller->frame; }
    void StartScriptingEntity(const CScriptThing& t, Handle&, long pri) { Log(&t == puller && pri == 4 ? "start" : "badstart"); }
    int GetThingWithScriptName(const char* n) { return std::strcmp(n, "SwordOfStrength") == 0 ? 7 : 0; }
    void SetThingAsUsable(int t, bool b) { Log(t == 7 && b ? "usable" : "badusable"); }
};
static int g_rand;
static int my_rand() { return g_rand; }
struct CV_SwordInTheStoneScript {
    CGameScriptInterface* Game;
    bool CrowdAttemptingToPullSword, CrowdSwordPullActive; long CrowdSwordTimer;
    int termAfter;
    bool IsActiveThreadTerminating() { return --termAfter < 0; }
    void SetPredicate(const CScriptThing& t) { Log("pred"); }
    void WatchForDeathDuringSwordPull(CScriptThing& puller)
    {
        SetPredicate(puller);
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return;
        for (;;) {
            if (IsActiveThreadTerminating()) return;
            if (puller.MsgIsKilledBy("") || puller.MsgIsHitBy("SCRIPT_NAME_HERO") || puller.MsgIsHitByAnyAggressiveSpecialAbilityFrom("SCRIPT_NAME_HERO")) {
                if (IsActiveThreadTerminating()) return;
                Handle handle;
                Game->StartScriptingEntity(puller, handle, 4);
                handle.ClearCommands();
                CrowdAttemptingToPullSword = false;
                CrowdSwordPullActive = false;
                Game->SetThingAsUsable(Game->GetThingWithScriptName("SwordOfStrength"), true);
                CrowdSwordTimer = my_rand() % 6 + 10;
                return;
            }
            Game->NewScriptFrame();
        }
    }
};
static bool Run(int evAt, int ev, int term, int rnd, const char* expect, bool expectFired, long expectTimer)
{
    int evs[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };
    evs[evAt] = ev;
    g_log[0] = 0; g_rand = rnd;
    CScriptThing p; p.frame = 0; p.ev = evs;
    CGameScriptInterface g; g.puller = &p;
    CV_SwordInTheStoneScript s; s.Game = &g; s.CrowdAttemptingToPullSword = true; s.CrowdSwordPullActive = true;
    s.CrowdSwordTimer = -1; s.termAfter = term;
    s.WatchForDeathDuringSwordPull(p);
    bool fired = !s.CrowdAttemptingToPullSword && !s.CrowdSwordPullActive;
    return std::strcmp(g_log, expect) == 0 && fired == expectFired && s.CrowdSwordTimer == expectTimer;
}
int main()
{
    bool ok = Run(3, 1, 100, 17, "pred;frame;frame;frame;start;clear;usable;", true, 15)
        && Run(2, 2, 100, 6, "pred;frame;frame;start;clear;usable;", true, 10)
        && Run(4, 3, 100, 8, "pred;frame;frame;frame;frame;start;clear;usable;", true, 12)
        && Run(5, 1, 3, 0, "pred;frame;frame;frame;", false, -1)
        && Run(1, 2, 0, 0, "pred;frame;", false, -1);
    if (!ok) { std::printf("00ed7320_TEST FAIL\n"); return 1; }
    std::printf("00ed7320_TEST PASS\n");
    return 0;
}
