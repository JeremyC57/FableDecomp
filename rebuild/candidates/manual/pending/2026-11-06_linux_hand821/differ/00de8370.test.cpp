// Behaviour test for NScript::CQ_RansomVictimScript::CRansommer::Main (0x00DE8370): on the first meeting play the cave cut-scene and
// ask for the ransom (7500, or 10000 once his helpers were attacked): yes with enough gold pays and frees the victim, yes without
// says NOT_ENOUGH_GOLD, no plays the refused cut-scene; afterwards each talk asks again (or says DONE once the victim is free) and a
// hero hit turns him bandit with the on-hit line for good.
#include <cstdio>
#include <cstring>
static char g_log[4096];
static void Log(const char* s) { std::strcat(g_log, s); std::strcat(g_log, ";"); }
struct Ransommer {
    int Frames, TerminateAt, Gold, TalkMask, HitAt, Answers[4], NextAnswer;
    bool Rescued, HelpersAttacked;
    bool T() const { return Frames >= TerminateAt; }
    void NewScriptFrame() { ++Frames; }
    bool Talked() const { return Frames < 31 && ((TalkMask >> Frames) & 1); }
    void Ask()
    {
        long price = HelpersAttacked ? 10000 : 7500;
        if (Answers[NextAnswer++] == 1) {
            if (Gold >= price) { Gold -= price; Rescued = true; Log("PAID"); } else Log("NOT_ENOUGH_GOLD");
        } else Log("REFUSED");
    }
    void Main()
    {
        NewScriptFrame(); if (T()) return;
        if (!Rescued) { Log(HelpersAttacked ? "CS_1B" : "CS_1A"); Ask(); }
        while (!T()) {
            if (Talked()) { if (!Rescued) { Log("ASK"); Ask(); } else Log("DONE"); }
            if (Frames == HitAt) { Log("BANDIT"); Log("ON_HIT"); for (;;) { NewScriptFrame(); if (T()) return; } }
            NewScriptFrame();
        }
    }
};
int main()
{
    Ransommer a = { 0, 12, 8000, (1 << 3) | (1 << 5), -1, { 2, 1, 0, 0 }, 0, false, false };
    a.Main();
    bool ok = !std::strcmp(g_log, "CS_1A;REFUSED;ASK;PAID;DONE;") && a.Rescued && a.Gold == 500;
    g_log[0] = 0;
    Ransommer b = { 0, 9, 9000, (1 << 2), 4, { 1, 1, 0, 0 }, 0, false, true };
    b.Main();
    ok = ok && !std::strcmp(g_log, "CS_1B;NOT_ENOUGH_GOLD;ASK;NOT_ENOUGH_GOLD;BANDIT;ON_HIT;") && !b.Rescued && b.Frames == 9;
    std::printf(ok ? "00de8370_TEST PASS\n" : "00de8370_TEST FAIL\n");
    return ok ? 0 : 1;
}
