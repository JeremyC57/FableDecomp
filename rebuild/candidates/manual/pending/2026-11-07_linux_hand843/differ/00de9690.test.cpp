// Behaviour test for NScript::CQ_RansomVictimScript::CRansomHelper::Main (0x00DE9690): wait until the helper starts or the hero is within
// 10 units (being hit turns the helper bandit for good); play CS_RANSOM_CAVE_EXTERIOR_1 when the father's guard lives (and tell him to wait
// outside), else _2, open the demon door; then until the ransommer dies: after any hit wait 8 ticks and walk home, answer the hero when not
// walking home (_RESCUED_VICTIM_10 once the victim is rescued, else _YET_TO_PAY_10); a hero hit or the ransommer's death turns him bandit.
#include <cstdio>
#include <cstring>
static char g_log[1024];
static void Log(const char* s) { std::strcat(g_log, s); std::strcat(g_log, ";"); }
struct Helper {
    int Frame, End, Dist, HeroHitFrame, AnyHitFrame, TalkFrame, RescueFrame, DeathFrame, Timer, Home;
    bool Started, Spoken, Attacked, GuardAlive, ToldGuard, Rescued, RansommerDied, Bandit;
    bool T() const { return Frame >= End; }
    void NSF()
    {
        ++Frame; Dist -= 5;
        if (Timer > 0) --Timer;
        if (Frame == RescueFrame) Rescued = true;
        if (Frame == DeathFrame) RansommerDied = true;
    }
    void TurnBandit() { Bandit = true; Log("bandit"); }
    void Main()
    {
        NSF();
        if (T()) return;
        while (!Started && !(Dist < 10)) {
            NSF();
            if (T()) return;
            if (Frame == HeroHitFrame || Attacked) {
                Attacked = true; TurnBandit();
                while (true) { NSF(); if (T()) return; }
            }
        }
        if (!Started) {
            Started = true;
            NSF();
            if (T()) return;
            if (GuardAlive) { Log("CS_RANSOM_CAVE_EXTERIOR_1"); ToldGuard = true; }
            else Log("CS_RANSOM_CAVE_EXTERIOR_2");
            Log("open_door");
            Spoken = true;
        }
        bool goingHome = false;
        while (!RansommerDied) {
            NSF();
            if (T()) return;
            if (Frame == HeroHitFrame || Attacked) { Attacked = true; break; }
            if (Frame == AnyHitFrame) { Timer = 8; goingHome = true; }
            if (goingHome && Timer == 0) {
                if (Home > 0) --Home;
                if (Home < 2) { Log("home"); goingHome = false; }
            }
            if (Frame == TalkFrame && !goingHome) Log(Rescued ? "_RESCUED_VICTIM_10" : "_YET_TO_PAY_10");
        }
        TurnBandit();
        while (true) { NSF(); if (T()) return; }
    }
};
int main()
{
    Helper a = { 0, 40, 30, -1, 8, 20, 25, 30, 0, 3, false, false, false, true, false, false, false, false };
    a.Main();
    bool ok = !std::strcmp(g_log, "CS_RANSOM_CAVE_EXTERIOR_1;open_door;home;_YET_TO_PAY_10;bandit;") && a.ToldGuard && a.Frame == 40;
    g_log[0] = 0;
    Helper b = { 0, 40, 100, 3, -1, -1, -1, -1, 0, 0, false, false, false, false, false, false, false, false };
    b.Main();
    ok = ok && !std::strcmp(g_log, "bandit;") && b.Attacked && b.Frame == 40;
    g_log[0] = 0;
    Helper c = { 0, 40, 30, -1, -1, 9, 7, 12, 0, 0, false, false, false, false, false, false, false, false };
    c.Main();
    ok = ok && !std::strcmp(g_log, "CS_RANSOM_CAVE_EXTERIOR_2;open_door;_RESCUED_VICTIM_10;bandit;");
    std::printf(ok ? "00de9690_TEST PASS\n" : "00de9690_TEST FAIL\n");
    return ok ? 0 : 1;
}
