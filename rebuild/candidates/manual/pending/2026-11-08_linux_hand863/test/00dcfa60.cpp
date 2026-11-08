// Behaviour test for 0x00DCFA60 NScript::CQ_OrchardFarmRaidScript::DoCutsceneIfRequired: a portable model of the script thread --
// the team spawn point chosen by HeroTeam, the per-frame wait for the Orchard Farm region, the wrong-entrance branch (gates
// closed and locked, info shown, wait for the click and for the hero to leave, flag set then cleared), and at the right entrance
// the evil (bandits + nearest guard, CS_ORCHARD_EVIL_INTRO) or good (hero only, CS_ORCHARD_GOOD_INTRO) intro, the quest start
// screen, the gates opened, the barricade quest deactivated, DoneIntroduction set and the farmhouse door locked.
#include <cstdio>
#include <string>
#include <vector>

struct World {
    int frame; int terminateAt; int heroTeam;
    std::vector<int> regionLoaded;   // per frame
    std::vector<int> nearSpawn;      // per frame
    int clickFrame;
    std::vector<std::string> log;
    bool doneIntro, wrongEntrance; int gatesClosedCount;
    bool T() const { return frame >= terminateAt; }
    void NSF() { ++frame; }
    bool Region() const { return frame < (int)regionLoaded.size() ? regionLoaded[frame] != 0 : true; }
    bool Near() const { return frame < (int)nearSpawn.size() ? nearSpawn[frame] != 0 : true; }
};

static void DoCutsceneIfRequired(World& w)
{
    std::string spawn;
    if (w.heroTeam == 1) { if (w.T()) return; spawn = "BanditTeamSpawn"; }
    else { if (w.T()) return; spawn = "GuardTeamSpawn"; }
    w.log.push_back(spawn);
    for (; !w.T(); w.NSF()) {
        while (!w.Region()) { w.NSF(); if (w.T()) return; }
        if (w.T()) return;
        if (w.Near()) {
            if (w.T()) return;
            if (w.heroTeam == 1) {
                if (w.T()) return;
                w.log.push_back("CS_ORCHARD_EVIL_INTRO"); w.log.push_back("Q_OrchardFarmRaidEvil"); w.log.push_back("open");
            } else {
                if (w.T()) return;
                w.log.push_back("open"); w.log.push_back("CS_ORCHARD_GOOD_INTRO"); w.log.push_back("Q_OrchardFarmRaidGood");
            }
            w.log.push_back("deactivate Q_OrchardFarm_Barricade");
            w.doneIntro = true;
            w.log.push_back("lock OFFarmhouseDoor");
            return;
        }
        if (w.T()) return;
        w.wrongEntrance = true; ++w.gatesClosedCount; w.log.push_back("TEXT_QST_051_WRONG_ENTRANCE");
        while (w.frame < w.clickFrame) { w.NSF(); if (w.T()) return; }
        if (w.T()) return;
        while (w.Region()) { w.NSF(); if (w.T()) return; }
        if (w.T()) return;
        w.wrongEntrance = false;
    }
}

int main()
{
    bool ok = true;
    {   // evil hero, right entrance after the region loads on frame 2
        World w = { 0, 100, 1, {0, 0, 1}, {1}, 0, {}, false, false, 0 };
        w.nearSpawn.assign(10, 1);
        DoCutsceneIfRequired(w);
        ok = ok && w.doneIntro && w.log.size() == 6 && w.log[0] == "BanditTeamSpawn" && w.log[1] == "CS_ORCHARD_EVIL_INTRO"
                && w.log[3] == "open" && w.frame == 2 && w.gatesClosedCount == 0;
    }
    {   // good hero: wrong entrance first, leaves the region, comes back right
        World w = { 0, 100, 0, {1, 1, 1, 1, 0, 0, 1, 1, 1, 1}, {0, 0, 0, 0, 0, 0, 1, 1, 1, 1}, 3, {}, false, false, 0 };
        DoCutsceneIfRequired(w);
        ok = ok && w.doneIntro && !w.wrongEntrance && w.gatesClosedCount == 1 && w.log[0] == "GuardTeamSpawn"
                && w.log[1] == "TEXT_QST_051_WRONG_ENTRANCE" && w.log[2] == "open" && w.log[3] == "CS_ORCHARD_GOOD_INTRO";
    }
    {   // terminated while waiting for the region
        World w = { 0, 3, 0, {0, 0, 0, 0, 0}, {1}, 0, {}, false, false, 0 };
        DoCutsceneIfRequired(w);
        ok = ok && !w.doneIntro && w.log.size() == 1;
    }
    std::printf(ok ? "00dcfa60_TEST PASS\n" : "00dcfa60_TEST FAIL\n");
    return ok ? 0 : 1;
}
