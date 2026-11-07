// Behaviour test for NScript::CQ_WhiteBalverineWWScript::CKG_Chief::Main (0x00E19D80): the Knothole Glade chief during the white
// balverine war. After setup (unkillable, not counted, friends with everything, scripted, attached to the active quest) he loops until
// MissionSucceeded: a talk while the white balverine is dead plays CS_WBK_CHIEF4 (cast HERO / CHIEF, camera fixed around it) and sets
// MissionSucceeded; a talk while it lives says BALVERINE_GONE_FIRST (InitialChatted false; retail stores false again) or _SECOND; a
// hit by the hero (but not a heal-life spell) says BEEN_ATTACKED and restores the friends flag. Leaving the loop clears his marker.
// A state model of the control flow, with the frame on which the thread terminates.
#include <cstdio>
#include <cstring>
static char g_log[2048];
static void Log(const char* s) { std::strcat(g_log, s); std::strcat(g_log, ";"); }
struct Chief {
    int Frames, TerminateAt, TalkAt, HitAt, HealAt;
    bool BalverineAlive, MissionSucceeded, InitialChatted, Friends;
    void NewScriptFrame() { ++Frames; }
    bool T() const { return Frames >= TerminateAt; }
    void Speak(const char* line) { Log(line); NewScriptFrame(); }
    void Main()
    {
        NewScriptFrame();
        if (T()) return;
        Log("setup"); Friends = true;
        while (!MissionSucceeded) {
            NewScriptFrame();
            if (T()) return;
            if (Frames == TalkAt) {
                if (!BalverineAlive) {
                    Log("CS_WBK_CHIEF4[HERO,CHIEF]");
                    MissionSucceeded = true;
                } else if (!InitialChatted) {
                    InitialChatted = false;
                    Speak("TEXT_QST_074_CHIEF_BALVERINE_GONE_FIRST");
                    if (T()) return;
                } else {
                    Speak("TEXT_QST_074_CHIEF_BALVERINE_GONE_SECOND");
                    if (T()) return;
                }
            }
            if (Frames == HitAt && Frames != HealAt) {
                Friends = false;
                NewScriptFrame();
                if (T()) return;
                Speak("TEXT_QST_074_CHIEF_BEEN_ATTACKED");
                if (T()) return;
                NewScriptFrame();
                if (T()) return;
                Friends = true;
            }
        }
        if (T()) return;
        Log("ClearThingHasInformation");
    }
};
int main()
{
    Chief a = { 0, 40, 3, -1, -1, true, false, false, false };
    a.Main();
    bool ok = !std::strcmp(g_log, "setup;TEXT_QST_074_CHIEF_BALVERINE_GONE_FIRST;") && !a.InitialChatted && !a.MissionSucceeded;
    g_log[0] = 0;
    Chief b = { 0, 40, 3, -1, -1, false, false, false, false };
    b.Main();
    ok = ok && !std::strcmp(g_log, "setup;CS_WBK_CHIEF4[HERO,CHIEF];ClearThingHasInformation;") && b.MissionSucceeded;
    g_log[0] = 0;
    Chief c = { 0, 12, -1, 2, -1, true, false, true, false };
    c.Main();
    ok = ok && !std::strcmp(g_log, "setup;TEXT_QST_074_CHIEF_BEEN_ATTACKED;") && c.Friends;
    g_log[0] = 0;
    Chief d = { 0, 12, -1, 2, 2, true, false, true, false };
    d.Main();
    ok = ok && !std::strcmp(g_log, "setup;") && d.Friends;
    std::printf(ok ? "00e19d80_TEST PASS\n" : "00e19d80_TEST FAIL\n");
    return ok ? 0 : 1;
}