// Behaviour test for CSummonerMinion::Main (0x00DF2AB0): the call sequence of the minion script.
#include <cstdio>
static char g_log[64]; static int g_n = 0;
struct Parent { long SummonersAlive; long CurrentAttackWave; bool SummonerAttacksStarted; };
struct Minion {
    Parent* ParentClass; long WaveID; int frames; int doneAfter; int startAt; int deadAt;
    void log(char c) { if (g_n < 63) g_log[g_n++] = c; }
    bool IsDone() { return frames >= doneAfter; }
    void NewScriptFrame()
    {
        ++frames;
        if (frames == startAt) { ParentClass->SummonerAttacksStarted = true; ParentClass->CurrentAttackWave = WaveID; }
        if (frames == deadAt) ParentClass->SummonersAlive = 0;
    }
    void Main()
    {
        NewScriptFrame();
        if (IsDone()) return;
        log('a'); log('L');
        while (!(ParentClass->SummonerAttacksStarted && ParentClass->CurrentAttackWave == WaveID)) {
            NewScriptFrame();
            if (IsDone()) return;
        }
        if (IsDone()) return;
        if (WaveID == 1) { if (IsDone()) return; log('T'); }
        log('l'); log('F');
        while (ParentClass->SummonersAlive) { NewScriptFrame(); if (IsDone()) return; }
        if (IsDone()) return;
        log('P'); log('R');
        do { NewScriptFrame(); } while (!IsDone());
    }
};
int main()
{
    Parent p = { 3, 0, false };
    Minion m = { &p, 1, 0, 50, 4, 9 };
    m.Main();
    g_log[g_n] = 0;
    const char* want = "aLTlFPR";
    for (int i = 0; ; ++i) { if (g_log[i] != want[i]) { std::printf("00df2ab0_TEST FAIL\n"); return 1; } if (!want[i]) break; }
    if (m.frames != 50) { std::printf("00df2ab0_TEST FAIL\n"); return 1; }
    g_n = 0; Parent q = { 3, 0, false }; Minion n = { &q, 2, 0, 6, 4, 9 }; n.Main(); g_log[g_n] = 0;   // done while waiting
    const char* want2 = "aLlF";
    for (int i = 0; ; ++i) { if (g_log[i] != want2[i]) { std::printf("00df2ab0_TEST FAIL\n"); return 1; } if (!want2[i]) break; }
    std::printf("00df2ab0_TEST PASS\n");
    return 0;
}