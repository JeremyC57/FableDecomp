// Behaviour test for NScript::CV_ExposeMayorScript::CEM_Sheriff::Main (0x00E78630): the sheriff plays the intro once, then until
// stage 5 answers each talk with the married comment (once, if the hero married Lady Grey) and the stage's line (stage 1 and 3 say
// their first line once and the repeat line after; stage 4 path 1 asks the become-mayor question: yes sets stage 5, no sets the
// objective and says NOT_MAYOR), and answers a hit with the ally reset and ON_HIT; after stage 5 talk gives the path's stage-5 line.
#include <cstdio>
#include <cstring>
static char g_log[4096];
static void Log(const char* s) { std::strcat(g_log, s); std::strcat(g_log, ";"); }
struct Sheriff {
    int Frames, TerminateAt, Stage, Path, Answer, TalkMask, HitMask;
    bool Married, IntroDone, StageTwoSpoken, StageFourSpoken, Commented, SaidYes;
    bool T() const { return Frames >= TerminateAt; }
    void NewScriptFrame() { ++Frames; }
    bool Talked() const { return Frames < 31 && ((TalkMask >> Frames) & 1); }
    bool Hit() const { return Frames < 31 && ((HitMask >> Frames) & 1); }
    void Say(const char* s) { Log(s); }
    void Main()
    {
        NewScriptFrame(); if (T()) return;
        if (!IntroDone) { Log("CS_INTRO"); IntroDone = true; }
        while (Stage != 5) {
            NewScriptFrame(); if (T()) return;
            if (Talked()) {
                if (!Commented && Married) { Commented = true; Say("MARRIED"); }
                if (Stage == 0) Say("STAGE0");
                else if (Stage == 1) { if (StageTwoSpoken) Say("STAGE2_REPEAT"); else { Say("STAGE2"); StageTwoSpoken = true; } }
                else if (Stage == 2) Say("STAGE2_REPEAT");
                else if (Stage == 3) { if (StageFourSpoken) Say("STAGE2_REPEAT"); else { Say("STAGE4"); StageFourSpoken = true; } }
                else if (Stage == 4) {
                    if (Path == 1) {
                        Say("STAGE5A"); Log("QUESTION");
                        if (Answer == 1) { SaidYes = true; Log("persist"); Stage = 5; }
                        else { Log("OBJECTIVE_06"); Say("NOT_MAYOR"); }
                    } else if (Path == 2) Say("STAGE5B");
                }
            }
            if (Hit()) { Log("ally"); Say("ON_HIT"); }
            if (Frames == 4 && Stage < 4) ++Stage;
        }
        if (T()) return;
        while (!T()) {
            if (Talked()) { if (Path == 1) Say("STAGE5A"); else if (Path == 2) Say("STAGE5B"); }
            if (Hit()) { Log("ally"); Say("ON_HIT"); }
            NewScriptFrame();
        }
    }
};
int main()
{
    Sheriff a = { 0, 12, 3, 1, 1, (1 << 2) | (1 << 3) | (1 << 5) | (1 << 9), (1 << 6), true, false, false, false, false, false };
    a.Main();
    bool ok = !std::strcmp(g_log, "CS_INTRO;MARRIED;STAGE4;STAGE2_REPEAT;STAGE5A;QUESTION;persist;STAGE5A;ally;ON_HIT;STAGE5A;")
        && a.SaidYes && a.Stage == 5;
    g_log[0] = 0;
    Sheriff b = { 0, 6, 0, 2, 0, (1 << 2) | (1 << 3), 0, false, true, false, false, false, false };
    b.Main();
    ok = ok && !std::strcmp(g_log, "STAGE0;STAGE0;") && b.Frames == 6 && !b.Commented;
    std::printf(ok ? "00e78630_TEST PASS\n" : "00e78630_TEST FAIL\n");
    return ok ? 0 : 1;
}