// Behaviour test for NScript::CQ_OrchardFarmRaidScript::DoCutsceneIfRequired (0x00DCFA60): the Orchard Farm intro thread. It picks
// the hero's team spawn (BanditTeamSpawn for team 1, GuardTeamSpawn otherwise), then every frame waits for the OrchardFarm region.
// When the hero is within 10 units of his team's spawn it plays the team's intro (evil: the nearest guard, two bandits and the hero
// scripted, weapons sheathed, CS_ORCHARD_EVIL_INTRO, the evil quest start screen, then the main gates open; good: the gates open
// first, then CS_ORCHARD_GOOD_INTRO and the good start screen), deactivates Q_OrchardFarm_Barricade in either arm, pauses a second,
// sets DoneIntroduction, locks the farmhouse door and ends. Otherwise it flags HeroAtWrongEntrance, closes and locks the main gates,
// shows TEXT_QST_051_WRONG_ENTRANCE, waits for the info to be clicked past and for the region to unload, clears the flag and starts
// again. Every wait ends the thread when it is terminating. A state model run for: evil intro, good intro, wrong entrance then the
// right one, a terminate inside the region wait, a terminate inside the info wait, and a late region load.
#include <cstdio>
#include <cstring>
static char g_log[4096];
static void Log(const char* s) { std::strcat(g_log, s); std::strcat(g_log, ";"); }
struct Intro {
    int HeroTeam, Frames, TerminateAt, RegionFrom, RegionUntil, RegionAgain, NearFrom, ClickAt;
    bool DoneIntroduction, HeroAtWrongEntrance;
    const char* Spawn;
    void NewScriptFrame() { ++Frames; }
    bool T() const { return Frames >= TerminateAt; }
    bool IsRegionLoaded() const { return (Frames >= RegionFrom && Frames < RegionUntil) || Frames >= RegionAgain; }
    bool NearSpawn() const { return Frames >= NearFrom; }
    bool ClickedPast() const { return Frames >= ClickAt; }
    void Run()
    {
        if (HeroTeam == 1) { if (T()) return; Spawn = "BanditTeamSpawn"; }
        else { if (T()) return; Spawn = "GuardTeamSpawn"; }
        for (;; NewScriptFrame()) {
            if (T()) return;
            while (!IsRegionLoaded()) { NewScriptFrame(); if (T()) return; }
            if (T()) return;
            if (NearSpawn()) {
                if (T()) return;
                if (HeroTeam == 1) {
                    if (T()) return;
                    Log("script guard+ban1+ban2+hero"); Log("sheathe"); Log("CS_ORCHARD_EVIL_INTRO");
                    Log("start Q_OrchardFarmRaidEvil"); Log("open gates");
                    Log("deactivate Q_OrchardFarm_Barricade");
                } else {
                    if (T()) return;
                    Log("open gates"); Log("script hero"); Log("CS_ORCHARD_GOOD_INTRO");
                    Log("start Q_OrchardFarmRaidGood");
                    Log("deactivate Q_OrchardFarm_Barricade");
                }
                Log("pause");
                DoneIntroduction = true;
                Log("lock farmhouse");
                return;
            }
            if (T()) return;
            HeroAtWrongEntrance = true;
            Log("close gates"); Log("lock gates"); Log("pause"); Log("TEXT_QST_051_WRONG_ENTRANCE");
            while (!ClickedPast()) { NewScriptFrame(); if (T()) return; }
            if (T()) return;
            while (IsRegionLoaded()) { NewScriptFrame(); if (T()) return; }
            if (T()) return;
            HeroAtWrongEntrance = false;
        }
    }
};
int main()
{
    Intro a = { 1, 0, 100, 0, 100, 100, 0, 0, false, false, 0 };
    a.Run();
    bool ok = !std::strcmp(g_log, "script guard+ban1+ban2+hero;sheathe;CS_ORCHARD_EVIL_INTRO;start Q_OrchardFarmRaidEvil;open gates;"
                                  "deactivate Q_OrchardFarm_Barricade;pause;lock farmhouse;")
              && a.DoneIntroduction && !a.HeroAtWrongEntrance && !std::strcmp(a.Spawn, "BanditTeamSpawn") && a.Frames == 0;
    g_log[0] = 0;
    Intro b = { 0, 0, 100, 3, 100, 100, 0, 0, false, false, 0 };
    b.Run();
    ok = ok && !std::strcmp(g_log, "open gates;script hero;CS_ORCHARD_GOOD_INTRO;start Q_OrchardFarmRaidGood;"
                                   "deactivate Q_OrchardFarm_Barricade;pause;lock farmhouse;")
         && b.DoneIntroduction && !std::strcmp(b.Spawn, "GuardTeamSpawn") && b.Frames == 3;
    g_log[0] = 0;
    Intro c = { 0, 0, 100, 2, 9, 14, 12, 5, false, false, 0 };      // wrong entrance at 2, clicked at 5, leaves at 9, back (right) at 14
    c.Run();
    ok = ok && !std::strcmp(g_log, "close gates;lock gates;pause;TEXT_QST_051_WRONG_ENTRANCE;open gates;script hero;CS_ORCHARD_GOOD_INTRO;"
                                   "start Q_OrchardFarmRaidGood;deactivate Q_OrchardFarm_Barricade;pause;lock farmhouse;")
         && c.DoneIntroduction && !c.HeroAtWrongEntrance && c.Frames == 14;
    g_log[0] = 0;
    Intro d = { 1, 0, 4, 50, 100, 100, 0, 0, false, false, 0 };     // terminates while waiting for the region
    d.Run();
    ok = ok && g_log[0] == 0 && !d.DoneIntroduction && !d.HeroAtWrongEntrance && d.Frames == 4;
    Intro e = { 1, 0, 6, 1, 100, 100, 50, 30, false, false, 0 };    // wrong entrance, terminates before the info is clicked past
    e.Run();
    ok = ok && !std::strcmp(g_log, "close gates;lock gates;pause;TEXT_QST_051_WRONG_ENTRANCE;") && e.HeroAtWrongEntrance
         && !e.DoneIntroduction && e.Frames == 6;
    g_log[0] = 0;
    Intro f = { 1, 0, 0, 0, 100, 100, 0, 0, false, false, 0 };      // terminating before the spawn lookup
    f.Run();
    ok = ok && g_log[0] == 0 && f.Spawn == 0 && !f.DoneIntroduction;
    std::printf(ok ? "00dcfa60_TEST PASS\n" : "00dcfa60_TEST FAIL\n");
    return ok ? 0 : 1;
}
