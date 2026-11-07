// Behaviour test for NScript::CQ_GuildTrainingScript::CAppleGirl::Main (0x00D3D150): the apple girl at the Guild. While no apple hunt
// is running she calls HELP to a nearby hero whenever her 15-frame chat timer runs out; talked to she chats once (CHAT + QUESTION, later
// QUESTION_AGAIN), a yes starts the hunt (child mode when the pre-melee quest is active), a no gets IMPLORE. During the hunt she is
// removed every frame once the pre-melee quest ends in child mode; talked to she takes up to 4 - CurrentApples red apples and answers
// ONE_MORE_APPLE / MANY_MORE_APPLES / NO_MORE_APPLES, then (apples given) THREE / TWO / ONE_NEEDED or, at four, THANKS + blueberry pie.
// A state model of the script's control flow.
#include <cstdio>
#include <cstring>
static char g_log[4096];
static void Log(const char* s) { std::strcat(g_log, s); std::strcat(g_log, ";"); }
struct AppleGirl {
    int Frames, TerminateAt, TalkAt[4], Answer[4], NTalk, QuestActiveUntil, HeroApples, RefillAt, RefillTo; bool Near;
    int AppleMode, CurrentApples, Timer, NTalkDone; bool ChildAppleMode, HaveChatted;
    bool T() const { return Frames >= TerminateAt; }
    void Frame() { ++Frames; if (Timer > 0) --Timer; if (Frames == RefillAt) HeroApples = RefillTo; }
    bool Talked() { for (int i = 0; i < NTalk; ++i) if (TalkAt[i] == Frames) { NTalkDone = i; return true; } return false; }
    bool QuestActive() const { return Frames < QuestActiveUntil; }
    bool ChatDue() const { return Near && Timer <= 0; }
    void Main()
    {
        AppleMode = 0; Timer = 15; CurrentApples = 0; ChildAppleMode = false;
        while (AppleMode == 0) {
            Frame(); if (T()) return;
            if (ChatDue()) { Log("HELP"); Timer = 15; }
            if (Talked()) {
                if (HaveChatted) Log("QUESTION_AGAIN");
                else { HaveChatted = true; Log("CHAT"); Log("QUESTION"); }
                if (Answer[NTalkDone] == 1) { AppleMode = 1; if (QuestActive()) ChildAppleMode = true; }
                else Log("IMPLORE");
            }
        }
        while (AppleMode == 1) {
            Frame(); if (T()) return;
            if (ChildAppleMode && !QuestActive()) Log("REMOVE");
            if (ChatDue()) { Log(CurrentApples ? "MORE_APPLES" : "ANY_APPLES"); Timer = 15; }
            if (Talked()) {
                int given = 0;
                while (HeroApples > 0 && CurrentApples + given < 4) { Frame(); if (T()) return; ++given; --HeroApples; }
                Log(given == 1 ? "ONE_MORE_APPLE" : given != 0 ? "MANY_MORE_APPLES" : "NO_MORE_APPLES");
                CurrentApples += given;
                if (given != 0) {
                    switch (CurrentApples) {
                    case 1: Log("THREE_NEEDED"); break;
                    case 2: Log("TWO_NEEDED"); break;
                    case 3: Log("ONE_NEEDED"); break;
                    case 4: Log("THANKS"); Log("PIE"); AppleMode = 2; break;
                    }
                }
            }
        }
        for (;; Frame()) {
            if (T()) return;
            if (ChatDue()) { Log("THANKS_AGAIN"); Timer = 15; }
        }
    }
};
int main()
{
    AppleGirl a = { 0, 25, { 3, 6, 10, 14 }, { 0, 1 }, 4, 1000, 1, 12, 5, false };
    a.Main();
    bool ok = !std::strcmp(g_log, "CHAT;QUESTION;IMPLORE;QUESTION_AGAIN;ONE_MORE_APPLE;THREE_NEEDED;MANY_MORE_APPLES;THANKS;PIE;")
        && a.AppleMode == 2 && a.CurrentApples == 4 && a.HeroApples == 2 && a.ChildAppleMode;
    g_log[0] = 0;
    AppleGirl b = { 0, 33, { 0 }, { 0 }, 0, 1000, 0, -1, 0, true };
    b.Main();
    ok = ok && !std::strcmp(g_log, "HELP;HELP;") && b.AppleMode == 0;
    g_log[0] = 0;
    AppleGirl c = { 0, 6, { 2, 5 }, { 1 }, 2, 4, 0, -1, 0, false };
    c.Main();
    ok = ok && !std::strcmp(g_log, "CHAT;QUESTION;REMOVE;REMOVE;NO_MORE_APPLES;") && c.CurrentApples == 0;
    std::printf(ok ? "00d3d150_TEST PASS\n" : "00d3d150_TEST FAIL\n");
    return ok ? 0 : 1;
}
