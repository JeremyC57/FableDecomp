// Behaviour test for CQ_WaspBossScript::WatchForCutscene (0x00E12330): the thread waits frame by frame for the queen
// hornet's attack flag, quits on a terminating thread, and otherwise plays CS_WASPBOSS_QUEEN with the hero cast as HERO
// inside a paused movie sequence, faces the hero to the queen, resets the camera and sets CutsceneFinished.
#include <stdio.h>
#include <string.h>
#include <map>
#include <string>

static int gFrames, gTerminateAtFrame = -1, gChecks;
static std::string gLog;

struct Script {
    bool QueenHornetAttacks;
    bool CutsceneFinished;
    int AttackOnFrame;

    void NewScriptFrame() { ++gFrames; if (gFrames == AttackOnFrame) QueenHornetAttacks = true; }
    bool IsActiveThreadTerminating() { ++gChecks; return gTerminateAtFrame >= 0 && gFrames >= gTerminateAtFrame; }

    void WatchForCutscene()
    {
        while (!QueenHornetAttacks) {
            NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
        }
        if (IsActiveThreadTerminating())
            return;
        std::string queen = "QueenHornet";
        gLog += "behaviour(" + queen + ",2);";
        std::string hero = "hero";
        gLog += "script(hero,4);";
        std::map<std::string, std::string> cast;
        cast["HERO"] = hero;
        gLog += "movie;pause(1);";
        gLog += "run(CS_WASPBOSS_QUEEN," + cast["HERO"] + ");";
        gLog += "face(hero," + queen + ",1);camera;";
        CutsceneFinished = true;
        gLog += "pause(0);";
    }
};

int main()
{
    int ok = 1;
    Script s = { false, false, 3 };
    s.WatchForCutscene();
    ok &= gFrames == 3 && s.CutsceneFinished;
    ok &= gLog == "behaviour(QueenHornet,2);script(hero,4);movie;pause(1);run(CS_WASPBOSS_QUEEN,hero);"
                  "face(hero,QueenHornet,1);camera;pause(0);";

    gFrames = 0; gLog.erase(); gTerminateAtFrame = 2;
    Script t = { false, false, 5 };
    t.WatchForCutscene();
    ok &= gFrames == 2 && !t.CutsceneFinished && gLog.empty();

    gFrames = 0; gLog.erase(); gTerminateAtFrame = 0;   // already attacking but terminating: nothing plays
    Script u = { true, false, 1 };
    u.WatchForCutscene();
    ok &= gFrames == 0 && !u.CutsceneFinished && gLog.empty();

    if (!ok) { printf("00e12330_TEST FAIL\n"); return 1; }
    printf("00e12330_TEST PASS\n");
    return 0;
}