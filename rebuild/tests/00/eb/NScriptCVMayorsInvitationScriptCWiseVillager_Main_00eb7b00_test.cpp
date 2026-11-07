// Behaviour test for NScript::CV_MayorsInvitationScript::CWiseVillager::Main (0x00EB7B00): until Thunder is beaten or lost, wait to be
// talked to (the first three hero hits re-take control, the fourth detaches the villager and idles it), then give the WhichQuest hint
// (HINT_5_ALT after the Lady Grey deal); afterwards speak the alternative line, ask the proposal and on YES accept, set marryable /
// follower and MissionFailed; finally clear information and reset the narrator. A frame-stepped model (the Thunder flags are only
// re-read after a hint, so a talk that lands after the duel ends still gets a second hint).
#include <cstdio>
#include <cstring>
static char g_log[2048];
static void Log(const char* s) { std::strcat(g_log, s); std::strcat(g_log, ";"); }
struct Villager {
    int Frame, TerminateAt, TalkAt, HitAt, ThunderAt, AnswerAt, Answer, WhichQuest, NumTimesHit, HitsSoFar;
    bool Deal, Defeated, Lost, MissionFailed;
    void NSF() { ++Frame; if (Frame == ThunderAt) Defeated = true; }
    bool T() const { return Frame >= TerminateAt; }
    bool Talked() { if (Frame == TalkAt) { TalkAt += 5; return true; } return false; }
    bool Hit() const { return HitAt > 0 && Frame >= HitAt && Frame < HitAt + 4; }
    void Speak(const char* s) { Log(s); }
    void Main()
    {
        NSF(); if (T()) return;
        Log("info"); 
        while (!Defeated && !Lost) {
            NSF(); if (T()) return;
            while (!Talked()) {
                NSF(); if (T()) return;
                if (Hit() && NumTimesHit < 3) { if (T()) return; ++NumTimesHit; }
                else if (Hit()) { if (T()) return; Log("detach"); while (true) { NSF(); if (T()) return; } }
            }
            if (T()) return;
            if (Deal && WhichQuest == 5) { if (T()) return; Speak("HINT_5_ALT"); }
            else { if (T()) return; char b[32]; std::sprintf(b, "HINT_%d", WhichQuest); Speak(b); }
        }
        if (T()) return;
        while (!Talked()) { NSF(); if (T()) return; }
        if (T()) return;
        Speak("ALTERNATIVE");
        Log("ask");
        long answer;
        while ((answer = Frame >= AnswerAt ? Answer : -1) < 0) { NSF(); if (T()) return; }
        if (T()) return;
        if (answer == 1) { if (T()) return; Speak("ACCEPTED"); Log("marry"); MissionFailed = true; }
        Log("clear");
    }
};
int main()
{
    Villager a = { 0, 200, 4, 0, 6, 0, 1, 2, 0, 0, false, false, false, false };
    a.AnswerAt = 12;
    a.Main();
    bool ok = !std::strcmp(g_log, "info;HINT_2;HINT_2;ALTERNATIVE;ask;ACCEPTED;marry;clear;") && a.MissionFailed;
    g_log[0] = 0;
    Villager b = { 0, 200, 50, 3, 100, 0, 0, 5, 0, 0, true, false, false, false };
    b.TerminateAt = 30;
    b.Main();
    ok = ok && !std::strcmp(g_log, "info;detach;") && b.NumTimesHit == 3 && b.Frame == 30;
    g_log[0] = 0;
    Villager c = { 0, 200, 3, 0, 4, 9, 0, 5, 0, 0, true, false, false, false };
    c.Main();
    ok = ok && !std::strcmp(g_log, "info;HINT_5_ALT;HINT_5_ALT;ALTERNATIVE;ask;clear;") && !c.MissionFailed;
    std::printf(ok ? "00eb7b00_TEST PASS\n" : "00eb7b00_TEST FAIL\n");
    return ok ? 0 : 1;
}