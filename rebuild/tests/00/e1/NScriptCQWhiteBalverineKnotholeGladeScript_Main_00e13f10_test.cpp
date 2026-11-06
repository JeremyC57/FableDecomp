// Behaviour test for NScript::CQ_WhiteBalverineKnotholeGladeScript::Main (0x00E13F10): the BalverineState stage machine. A fresh start
// activates the gates quest, every start sets objective 1, binds six scripts, puts the village in limbo, shows the start screen once and
// spawns WatchForTermination; stage 0/1 waits for the gates quest, 2 respawns a dead balverine and waits for 3, 3 plays CS_WBK_CHIEF1 and
// waits for the hero at the ambush marker, 4 respawns the balverine above the escape point, 5 plays CS_WBK_CHIEF2 with the reward and the
// tutorial, 6 respawns the balverine and the guards, 7 plays CS_WBK_CHIEF3 and succeeds. A state model, run from stage 0, from a reload
// at stage 5 and for a termination.
#include <cstdio>
#include <cstring>
static char g_log[4096];
static void Log(const char* s) { std::strcat(g_log, s); std::strcat(g_log, ";"); }
struct Quest {
    long BalverineState;
    bool QuestStartScreenShown, MissionSucceeded, GreenWife, WifeCutsceneStart, WifeCutsceneStop, BalverineAlive;
    int Frames, TerminateAt;
    void NewScriptFrame()
    {
        ++Frames;
        if (Frames > 1 && (BalverineState == 2 || BalverineState == 4 || BalverineState == 6)) ++BalverineState;   // the entity scripts advance it
    }
    bool T() { return Frames >= TerminateAt; }
    void Main()
    {
        while (Frames < 1) { NewScriptFrame(); if (T()) return; }
        if (T()) return;
        if (BalverineState == 0) { if (T()) return; Log("gates"); }
        Log("obj01"); Log("bind6"); Log("limbo");
        if (!QuestStartScreenShown) { if (T()) return; QuestStartScreenShown = true; Log("start"); }
        Log("watch");
        if (BalverineState == 0 || BalverineState == 1) {
            if (T()) return;
            BalverineState = 1;
            while (Frames < 2) { NewScriptFrame(); if (T()) return; }
            if (T()) return;
            BalverineState = 2;
        }
        if (BalverineState == 2) {
            if (T()) return;
            if (!BalverineAlive) { if (T()) return; Log("spawn_balverine"); }
            Log("obj02");
            while (BalverineState != 3) { NewScriptFrame(); if (T()) return; }
            if (T()) return;
            Log("music_off");
        }
        if (BalverineState == 3) {
            if (T()) return;
            NewScriptFrame(); if (T()) return;
            Log("CS_WBK_CHIEF1"); GreenWife = true; Log("obj03"); Log("obj04");
            BalverineState = 4;
        }
        if (BalverineState == 4) {
            if (T()) return;
            if (!BalverineAlive) { if (T()) return; Log("spawn_escape"); }
            while (BalverineState != 5) { NewScriptFrame(); if (T()) return; }
            if (T()) return;
            Log("music_off");
        }
        if (BalverineState == 5) {
            if (T()) return;
            NewScriptFrame(); if (T()) return;
            WifeCutsceneStart = true; Log("CS_WBK_CHIEF2"); WifeCutsceneStop = true;
            Log("obj05"); Log("reward"); Log("tutorial");
            BalverineState = 6;
        }
        if (BalverineState == 6) {
            if (T()) return;
            if (!BalverineAlive) { if (T()) return; Log("spawn_escape2"); }
            Log("guards");
            while (BalverineState != 7) { NewScriptFrame(); if (T()) return; }
            if (T()) return;
            Log("music_off");
        }
        if (BalverineState == 7) {
            if (T()) return;
            Log("CS_WBK_CHIEF3");
            MissionSucceeded = true;
        }
    }
};
int main()
{
    Quest a = { 0, false, false, false, false, false, false, 0, 100 };
    a.Main();
    bool ok = !std::strcmp(g_log, "gates;obj01;bind6;limbo;start;watch;spawn_balverine;obj02;music_off;CS_WBK_CHIEF1;obj03;obj04;spawn_escape;"
        "music_off;CS_WBK_CHIEF2;obj05;reward;tutorial;spawn_escape2;guards;music_off;CS_WBK_CHIEF3;")
        && a.MissionSucceeded && a.GreenWife && a.WifeCutsceneStart && a.WifeCutsceneStop && a.BalverineState == 7;
    g_log[0] = 0;
    Quest b = { 5, true, false, false, false, false, true, 0, 100 };
    b.Main();
    ok = ok && !std::strcmp(g_log, "obj01;bind6;limbo;watch;CS_WBK_CHIEF2;obj05;reward;tutorial;guards;music_off;CS_WBK_CHIEF3;")
        && b.MissionSucceeded && !b.GreenWife;
    g_log[0] = 0;
    Quest c = { 2, true, false, false, false, false, true, 0, 2 };
    c.Main();
    ok = ok && !std::strcmp(g_log, "obj01;bind6;limbo;watch;obj02;") && !c.MissionSucceeded;
    std::printf(ok ? "00e13f10_TEST PASS\n" : "00e13f10_TEST FAIL\n");
    return ok ? 0 : 1;
}