// Behaviour test for NScript::CV_BordelloScript::CBordelloClient::Main (0x00E44EA0): a female client walks to the stand and, when
// talked to, says her chat line (lines cycle 1..2) or, when hit, her on-hit line; a male client walks in (state 1 -> 2 at the
// stand, lines cycle 1..3), then in state 2 the timer or a nunnery sends him off (3); a talk while the hero tricks in a dress
// with sex chance 0 asks the question: yes pays 200, counts the sex, plays the quickie and sends him off; then in state 3 he walks
// to the spawn point (4) and fades out. A log model of the control flow.
#include <cstdio>
#include <cstring>
static char g_log[4096];
static void Log(const char* s) { std::strcat(g_log, s); std::strcat(g_log, ";"); }
struct World { int Frame, TerminateAt, ArriveAt, TalkAt, Talk2At, LeaveAt, HitAt, Answer, Gold, Sex; bool Tricking, Dressed, Nunnery; };
struct Client {
    World* W; bool Female; int BrainState, ClientID, SexChance, NextLine; bool Faded;
    bool T() { return W->TerminateAt > 0 && W->Frame >= W->TerminateAt; }
    void NSF() { ++W->Frame; }
    void Say(const char* tag) { char b[64]; std::sprintf(b, "%s%d", tag, NextLine); Log(b); }
    void Main()
    {
        NSF(); if (T()) return;
        if (Female) {
            bool arrived = false;
            for (; !T(); NSF()) {
                if (!arrived && W->Frame >= W->ArriveAt) arrived = true;
                if (W->Frame == W->TalkAt || W->Frame == W->Talk2At) { if (NextLine > 2) NextLine = 1; Say("chat"); NextLine++; }
                if (W->Frame == W->HitAt) Log("onhit");
            }
            return;
        }
        while (BrainState == 1) {
            NSF(); if (T()) return;
            if (W->Frame >= W->ArriveAt) BrainState = 2;
            if (W->Frame == W->TalkAt) { if (NextLine > 3) NextLine = 1; Say("enter"); NextLine++; }
        }
        if (T()) return;
        while (BrainState == 2) {
            NSF(); if (T()) return;
            if (W->Frame >= W->LeaveAt || W->Nunnery) BrainState = 3;
            if (W->Frame == W->Talk2At) {
                if (W->Tricking && W->Dressed) {
                    if (SexChance == 0) {
                        if (NextLine > 3) NextLine = 1; Say("interested"); NextLine++;
                        if (W->Answer == 1) { if (NextLine > 3) NextLine = 1; Say("accepted"); NextLine++; W->Gold += 200; W->Sex++; Log("quickie"); BrainState = 3; }
                        else { if (NextLine > 3) NextLine = 1; Say("declined"); NextLine++; }
                    } else { if (NextLine > 3) NextLine = 1; Say("disgusted"); NextLine++; }
                } else { if (NextLine > 3) NextLine = 1; Say("browse"); NextLine++; }
            }
        }
        if (T()) return;
        while (BrainState == 3) { NSF(); if (T()) return; BrainState = 4; }
        if (T()) return;
        if (BrainState == 4) Faded = true;
        for (;;) { NSF(); if (T()) return; }
    }
};
int main()
{
    World w = { 0, 10, 3, 2, 5, 0, 6, 0, 0, 0, false, false, false };
    Client f = { &w, true, 1, 0, 0, 2, false };
    g_log[0] = 0; f.Main();
    bool ok = !std::strcmp(g_log, "chat2;chat1;onhit;");
    World w2 = { 0, 20, 3, 2, 5, 50, 0, 1, 0, 0, true, true, false };
    Client m = { &w2, false, 1, 1, 0, 3, false };
    g_log[0] = 0; m.Main();
    ok = ok && !std::strcmp(g_log, "enter3;interested1;accepted2;quickie;") && w2.Gold == 200 && w2.Sex == 1 && m.Faded && m.BrainState == 4;
    World w3 = { 0, 20, 3, 0, 5, 9, 0, 0, 0, 0, false, false, false };
    Client b = { &w3, false, 1, 1, 0, 1, false };
    g_log[0] = 0; b.Main();
    ok = ok && !std::strcmp(g_log, "browse1;") && b.Faded && w3.Gold == 0;
    if (!ok) std::printf("%s\n", g_log);
    std::printf(ok ? "00e44ea0_TEST PASS\n" : "00e44ea0_TEST FAIL\n");
    return ok ? 0 : 1;
}