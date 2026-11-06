// Behaviour test for NScript::CQ_HobbeToothContestScript::Main (0x00D9A190): objective 1, wait for OrchardFarmEast, seven bindings and
// WatchForTermination, Whisper created at WhisperStart, the farmer and Whisper under scripted control, wait for the hero at the start
// marker (or a hit on Whisper / the farmer, which plays the farmer's on-hit line while he lives), objective 2, the intro cutscene, the
// start screen once, the timer, the outro set-up that kills the hobbes, the boast flag when the hero beat Whisper by more than
// HOB_BoastHobbeMany, then the won / draw / lost outro with its renown award. A state model, run for the three outcomes and a termination.
#include <cstdio>
#include <cstring>
static char g_log[4096];
static void Log(const char* s) { std::strcat(g_log, s); std::strcat(g_log, ";"); }
struct Quest {
    long Hero, Whisper, BoastMany;
    bool Hit, FarmerAlive, QuestStartScreenShown, CS_IntroDone, CS_OutroBegin, MissionSucceeded, MissionFailed, KillMore;
    int Frames, TerminateAt, Renown;
    void NewScriptFrame() { ++Frames; }
    bool T() { return Frames >= TerminateAt; }
    void Main()
    {
        Log("obj01");
        while (Frames < 1) { NewScriptFrame(); if (T()) return; }
        if (T()) return;
        Log("bind7"); Log("watch"); Log("whisper");
        while (Frames < 3 && !Hit) { NewScriptFrame(); if (T()) return; }
        if (T()) return;
        Log("obj02");
        if (Hit) { if (T()) return; if (FarmerAlive) Log("ON_HIT"); }
        Log("CS_HOBBESTOOTH_INTRO");
        if (!QuestStartScreenShown) { if (T()) return; Log("start"); QuestStartScreenShown = true; }
        CS_IntroDone = true; Log("timer"); CS_OutroBegin = true; Log("kill_hobbes");
        if (Hero > Whisper + BoastMany) { if (T()) return; KillMore = true; }
        if (Hero > Whisper) { if (T()) return; Log("WON"); MissionSucceeded = true; Renown = 30; }
        else if (Hero == Whisper) { if (T()) return; Log("DRAW"); MissionFailed = true; Renown = 20; }
        else { if (T()) return; Log("LOST"); MissionFailed = true; Renown = 10; }
    }
};
int main()
{
    Quest a = { 9, 3, 4, false, true, false, false, false, false, false, false, 0, 100, 0 };
    a.Main();
    bool ok = !std::strcmp(g_log, "obj01;bind7;watch;whisper;obj02;CS_HOBBESTOOTH_INTRO;start;timer;kill_hobbes;WON;")
        && a.MissionSucceeded && a.KillMore && a.Renown == 30 && a.CS_IntroDone && a.CS_OutroBegin && a.Frames == 3;
    g_log[0] = 0;
    Quest b = { 4, 4, 4, true, true, true, false, false, false, false, false, 0, 100, 0 };
    b.Main();
    ok = ok && !std::strcmp(g_log, "obj01;bind7;watch;whisper;obj02;ON_HIT;CS_HOBBESTOOTH_INTRO;timer;kill_hobbes;DRAW;")
        && b.MissionFailed && !b.KillMore && b.Renown == 20 && b.Frames == 1;
    g_log[0] = 0;
    Quest c = { 2, 5, 4, true, false, false, false, false, false, false, false, 0, 100, 0 };
    c.Main();
    ok = ok && !std::strcmp(g_log, "obj01;bind7;watch;whisper;obj02;CS_HOBBESTOOTH_INTRO;start;timer;kill_hobbes;LOST;") && c.Renown == 10;
    g_log[0] = 0;
    Quest d = { 2, 5, 4, false, false, false, false, false, false, false, false, 0, 2, 0 };
    d.Main();
    ok = ok && !std::strcmp(g_log, "obj01;bind7;watch;whisper;") && !d.MissionFailed && !d.MissionSucceeded;
    std::printf(ok ? "00d9a190_TEST PASS\n" : "00d9a190_TEST FAIL\n");
    return ok ? 0 : 1;
}