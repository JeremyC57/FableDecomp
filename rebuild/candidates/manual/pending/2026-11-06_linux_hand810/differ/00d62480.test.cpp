// Behaviour test for NScript::CQ_GuildTrainingWoodsDepartureScript::CArtifactThief::Main (0x00D62480): while he holds the lamp and is
// not attacked he comments to a near hero every 10 frames (first comment with a wave); a hero hit (not a heal spell) makes him drop the
// lamp to the hero and run; a talk gives the chat + lamp question (first time) or the return question: yes with enough gold buys the
// lamp (gold to the thief, he walks off), yes without gold / no keep it; once moving away a hit gives the attack line; then he fades.
#include <cstdio>
#include <cstring>
static char g_log[1024];
static void Log(const char* s) { std::strcat(g_log, s); std::strcat(g_log, ";"); }
struct Thief {
    int Frames, TerminateAt, Timer, Gold, Cost, TalkMask, HitMask, HealMask, Answer;
    bool Holding, Talked, NotAttacked, Near;
    bool T() const { return Frames >= TerminateAt; }
    void NSF() { ++Frames; if (Timer > 0) --Timer; }
    bool Bit(int m) const { return Frames < 31 && ((m >> Frames) & 1); }
    bool Hit() const { return Bit(HitMask) && !Bit(HealMask); }
    void Buy(const char* noLine)
    {
        if (Answer == 1) {
            if (Gold < Cost) Log("NO_GOLD");
            else { Log("YES"); Log("lamp"); Gold -= Cost; Holding = false; }
        } else Log(noLine);
    }
    void Main()
    {
        NSF(); if (T()) return;
        Holding = true; Talked = false; NotAttacked = true; Timer = 0;
        while (Holding && NotAttacked) {
            NSF(); if (T()) return;
            if (Near && Timer <= 0) { Log(Talked ? "COMMENT_SECOND" : "COMMENT_FIRST"); Timer = 10; }
            if (Hit()) { NotAttacked = false; Log(Talked ? "HIT" : "PRE_HIT"); Log("lamp"); Log("run"); }
            if (Bit(TalkMask) && Holding) {
                if (!Talked) { Talked = true; Log("CHAT"); Buy("QUESTION_NO"); }
                else { Log("RETURN"); Buy("RETURN_QUESTION_NO"); }
            }
        }
        if (T()) return;
        Log("fade");
    }
};
int main()
{
    Thief a = { 0, 8, 0, 50, 100, (1 << 3) | (1 << 6), 0, 0, 1, false, false, false, true };
    a.Main();
    bool ok = !std::strcmp(g_log, "COMMENT_FIRST;CHAT;NO_GOLD;RETURN;NO_GOLD;") && a.Holding;
    g_log[0] = 0;
    Thief b = { 0, 40, 0, 150, 100, (1 << 2) | (1 << 4), 0, 0, 0, false, false, false, false };
    b.Main();
    ok = ok && !std::strcmp(g_log, "CHAT;QUESTION_NO;RETURN;RETURN_QUESTION_NO;") && b.Gold == 150;
    g_log[0] = 0;
    Thief c = { 0, 40, 0, 150, 100, (1 << 2), (1 << 5) | (1 << 3), (1 << 3), 1, false, false, false, false };
    c.Main();
    ok = ok && !std::strcmp(g_log, "CHAT;YES;lamp;fade;") && c.Gold == 50 && !c.Holding;
    g_log[0] = 0;
    Thief d = { 0, 40, 0, 0, 100, 0, (1 << 3), 0, 0, false, false, false, false };
    d.Main();
    ok = ok && !std::strcmp(g_log, "PRE_HIT;lamp;run;fade;") && !d.NotAttacked;
    std::printf(ok ? "00d62480_TEST PASS\n" : "00d62480_TEST FAIL\n");
    return ok ? 0 : 1;
}
