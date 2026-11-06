// Behaviour test for NScript::CQ_OpeningGraveyardSecretPassageScript::Main (0x00DC27C0): reset, fourteen bindings, WatchForTermination;
// unless the demon door was opened: objective 1, wait for GraveYard_1, activate Q_UndeadRising, the start screen once, wait for the
// gravekeeper cutscene trigger, CS_OPENGRAVE_INTRO, the keeper's undead line when alive, wait for the skeleton visit, WatchForArmour,
// wait for finished_quest, drop the skeleton marker; then mark the door opened, wait for GraveyardPath, CheckDoor, wait for GraveYard_2,
// objective 5, wait for Q_UndeadRising to end and succeed. A state model, run for a fresh start, a reload and a termination.
#include <cstdio>
#include <cstring>
static char g_log[4096];
static void Log(const char* s) { std::strcat(g_log, s); std::strcat(g_log, ";"); }
struct Quest {
    bool finished_quest, skeleton_visited, GravekeeperIntroduced, GravekeeperCutsceneStart, UndeadCutscenePlayed, UndeadCutsceneStarted;
    bool MissionSucceeded, QuestStartScreenShown, HaveOpenedDemonDoor, KeeperAlive;
    int Frames, TerminateAt;
    void NewScriptFrame()
    {
        ++Frames;
        if (Frames == 2) GravekeeperCutsceneStart = true;
        if (Frames == 3) UndeadCutsceneStarted = true;
        if (Frames == 4) skeleton_visited = true;
        if (Frames == 5) finished_quest = true;
    }
    bool T() { return Frames >= TerminateAt; }
    bool Loaded(int at) { return Frames >= at; }
    void Main()
    {
        Log("init"); Log("bind14"); Log("watch");
        if (!HaveOpenedDemonDoor) {
            if (T()) return;
            Log("obj01");
            while (!Loaded(1)) { NewScriptFrame(); if (T()) return; }
            if (T()) return;
            Log("activate");
            if (!QuestStartScreenShown) { if (T()) return; Log("start"); QuestStartScreenShown = true; }
            while (!GravekeeperCutsceneStart) { NewScriptFrame(); if (T()) return; }
            if (T()) return;
            Log("CS_OPENGRAVE_INTRO");
            GravekeeperIntroduced = true;
            while (!UndeadCutsceneStarted) { NewScriptFrame(); if (T()) return; }
            if (T()) return;
            if (KeeperAlive) { if (T()) return; Log("undead_line"); }
            UndeadCutscenePlayed = true;
            while (!skeleton_visited) { NewScriptFrame(); if (T()) return; }
            if (T()) return;
            Log("armour");
            while (!finished_quest) { NewScriptFrame(); if (T()) return; }
            if (T()) return;
            Log("marker_off");
        }
        HaveOpenedDemonDoor = true;
        while (!Loaded(6)) { NewScriptFrame(); if (T()) return; }
        if (T()) return;
        Log("checkdoor");
        while (!Loaded(7)) { NewScriptFrame(); if (T()) return; }
        if (T()) return;
        Log("obj05");
        while (Frames < 8) { NewScriptFrame(); if (T()) return; }
        if (T()) return;
        MissionSucceeded = true;
    }
};
int main()
{
    Quest a = { false, false, false, false, false, false, false, false, false, true, 0, 100 };
    a.Main();
    bool ok = !std::strcmp(g_log, "init;bind14;watch;obj01;activate;start;CS_OPENGRAVE_INTRO;undead_line;armour;marker_off;checkdoor;obj05;")
        && a.MissionSucceeded && a.HaveOpenedDemonDoor && a.GravekeeperIntroduced && a.UndeadCutscenePlayed && a.Frames == 8;
    g_log[0] = 0;
    Quest b = { false, false, false, false, false, false, false, true, true, false, 0, 100 };
    b.Main();
    ok = ok && !std::strcmp(g_log, "init;bind14;watch;checkdoor;obj05;") && b.MissionSucceeded && !b.GravekeeperIntroduced && b.Frames == 8;
    g_log[0] = 0;
    Quest c = { false, false, false, false, false, false, false, false, false, false, 0, 3 };
    c.Main();
    ok = ok && !std::strcmp(g_log, "init;bind14;watch;obj01;activate;start;CS_OPENGRAVE_INTRO;") && !c.MissionSucceeded && !c.HaveOpenedDemonDoor;
    std::printf(ok ? "00dc27c0_TEST PASS\n" : "00dc27c0_TEST FAIL\n");
    return ok ? 0 : 1;
}