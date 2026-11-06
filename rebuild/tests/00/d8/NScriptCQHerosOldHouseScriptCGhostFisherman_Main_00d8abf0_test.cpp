// Behaviour test for NScript::CQ_HerosOldHouseScript::CGhostFisherman::Main (0x00D8ABF0): the ghost fisherman's loop. Until the wife
// is attacked or he is going: cry HELP to a near hero when the 15-second timer is clear (only before the first talk), abort the
// mission with a fade-out if the booty is dug up before the hero helps, and on talk play the cutscenes -- first talk: SETUP, INTRO_1,
// the intro question (yes: Helping, INTRO_2, quest card, objective 1, markers; no: REFUSAL); later talks: WHERE while the booty is
// still buried, SUCCESS (objective 4, GhostGoing) once helped, else COMPLAINT, or the return question when not helping. At the end the
// ghost effect + fade-out (wife attacked) or RemoveThing. A state model, run for the help/success path, a refusal, and the wife path.
#include <cstdio>
#include <cstring>
static char g_log[4096];
static void Log(const char* s) { std::strcat(g_log, s); std::strcat(g_log, ";"); }
struct Ghost {
    int Frames, TerminateAt, NearFrom, Timer, AnswerYes, DigAt, HelpedAt, WifeAt;
    int TalkFrames[8]; int NTalk;
    bool Helping, Helped, MissionAborted, WifeAttacked, GhostGoing, HaveTalked, BootyBuried;
    void NewScriptFrame()
    {
        ++Frames;
        if (Timer > 0) --Timer;
        if (Frames == DigAt) BootyBuried = false;
        if (Frames == HelpedAt) Helped = true;
        if (Frames == WifeAt) WifeAttacked = true;
    }
    bool T() const { return Frames >= TerminateAt; }
    bool Talked() const { for (int i = 0; i < NTalk; ++i) if (TalkFrames[i] == Frames) return true; return false; }
    void Main()
    {
        Timer = 0;
        while (!WifeAttacked && !GhostGoing) {
            NewScriptFrame();
            if (T()) return;
            if (!HaveTalked && Frames >= NearFrom && Timer <= 0) { Log("HELP"); Timer = 15; }
            if (!Helping && !BootyBuried) { MissionAborted = true; Log("fade"); }
            if (Talked()) {
                Log("SETUP");
                if (!HaveTalked) {
                    HaveTalked = true;
                    Log("INTRO_1");
                    if (AnswerYes) { Helping = true; Log("INTRO_2"); Log("card"); Log("obj01"); }
                    else Log("REFUSAL");
                } else if (Helping) {
                    if (BootyBuried) Log("WHERE");
                    else if (Helped) { Log("SUCCESS"); Log("obj04"); GhostGoing = true; }
                    else Log("COMPLAINT");
                } else {
                    if (AnswerYes) { Helping = true; Log("INTRO_2"); Log("card"); Log("obj01"); }
                    else Log("REFUSAL_LINE");
                }
            }
        }
        if (T()) return;
        if (WifeAttacked) { Log("Ghost_Appear_01"); Log("fade"); }
        else Log("remove");
    }
};
int main()
{
    Ghost a = { 0, 100, 1, 0, 1, 6, 8, -1, { 3, 5, 9 }, 3, false, false, false, false, false, false, true };
    a.Main();
    bool ok = !std::strcmp(g_log, "HELP;SETUP;INTRO_1;INTRO_2;card;obj01;SETUP;WHERE;SETUP;SUCCESS;obj04;remove;")
        && a.GhostGoing && a.Helping && !a.MissionAborted;
    g_log[0] = 0;
    Ghost b = { 0, 6, 1, 0, 0, 4, -1, -1, { 2 }, 1, false, false, false, false, false, false, true };
    b.Main();
    ok = ok && !std::strcmp(g_log, "HELP;SETUP;INTRO_1;REFUSAL;fade;fade;") && b.MissionAborted && b.Frames == 6;
    g_log[0] = 0;
    Ghost c = { 0, 100, 50, 0, 1, -1, -1, 3, { 2 }, 1, false, false, false, false, false, false, true };
    c.Main();
    ok = ok && !std::strcmp(g_log, "SETUP;INTRO_1;INTRO_2;card;obj01;Ghost_Appear_01;fade;") && c.WifeAttacked;
    std::printf(ok ? "00d8abf0_TEST PASS\n" : "00d8abf0_TEST FAIL\n");
    return ok ? 0 : 1;
}