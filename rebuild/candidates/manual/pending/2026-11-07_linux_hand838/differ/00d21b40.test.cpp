// Behaviour test for NScript::CQ_BreakSiegeScript::CBanditSiegeLeader::Main (0x00D21B40): the siege leader takes control; unless his
// demands are known he runs the two cut-scenes, activates the KnotholeGlade teleporter (the first match only) and kicks off the quest
// start screen once; then he marks the demands known, waits for the siege and the second wave, flags the hero's first attack and,
// once the mission succeeds, waits for the end cut-scene and removes himself. A frame-stepped model.
#include <cstdio>
#include <cstring>
static char g_log[1024];
static void Log(const char* s) { std::strcat(g_log, s); std::strcat(g_log, ";"); }
struct Leader {
    int Frame, TerminateAt, SiegeAt, WaveAt, HitAt, SuccessAt, EndAt, NumTele;
    bool DemandsKnown, Screened, SiegeStart, SecondWave, Attacked, Succeeded, EndDone, LetIn;
    const char* const* Tele;
    void NSF()
    {
        ++Frame;
        if (Frame == SiegeAt) SiegeStart = true;
        if (Frame == WaveAt) SecondWave = true;
        if (Frame == SuccessAt) Succeeded = true;
        if (Frame == EndAt) EndDone = true;
    }
    bool T() const { return Frame >= TerminateAt; }
    void Main()
    {
        NSF(); if (T()) return;
        if (!DemandsKnown) {
            Log("GIVEN"); Log("INTRO");
            for (int i = 0; i < NumTele; ++i) {
                if (T()) return;
                if (!std::strcmp(Tele[i], "KnotholeGlade")) { Log("tele"); break; }
            }
            if (!Screened) { Log("screen"); Screened = true; }
            LetIn = true;
            Log("objective");
        }
        if (!Screened) { Log("screen"); Screened = true; }
        DemandsKnown = true;
        while (!SiegeStart) { NSF(); if (T()) return; }
        while (!SecondWave) { NSF(); if (T()) return; }
        while (!T()) {
            if (!Attacked && Frame == HitAt) { Log("attacked"); Attacked = true; }
            if (Succeeded) {
                while (!EndDone) { NSF(); if (T()) return; }
                Log("remove");
                return;     // the model ends at RemoveThing (the predicate fails once he is gone)
            }
            NSF();
        }
    }
};
int main()
{
    static const char* const tele[] = { "Bowerstone", "KnotholeGlade", "KnotholeGlade" };
    Leader a = { 0, 100, 3, 5, 7, 9, 12, 3, false, false, false, false, false, false, false, false, tele };
    a.Main();
    bool ok = !std::strcmp(g_log, "GIVEN;INTRO;tele;screen;objective;attacked;remove;") && a.DemandsKnown && a.LetIn && a.Frame == 12;
    g_log[0] = 0;
    Leader b = { 0, 100, 2, 4, 50, 6, 6, 1, true, false, false, false, false, false, false, false, tele };
    b.Main();
    ok = ok && !std::strcmp(g_log, "screen;remove;") && !b.LetIn && b.Frame == 6;
    g_log[0] = 0;
    Leader c = { 0, 4, 10, 20, 50, 60, 70, 1, true, true, false, false, false, false, false, false, tele };
    c.Main();
    ok = ok && g_log[0] == 0 && c.Frame == 4;
    std::printf(ok ? "00d21b40_TEST PASS\n" : "00d21b40_TEST FAIL\n");
    return ok ? 0 : 1;
}
