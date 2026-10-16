#include <cstdio>
// Behaviour model of CTCMultiArrow::CancelForCutscene (0x007CC190) / ResumeForCutscene (0x007CC230): the cancelled flag
// toggles once and every ammo effect's emitter sleeps (cancel, particles and decals cleared) or wakes (resume).
struct Emitter { bool asleep; int cleared; };
struct Arrow {
    bool cancelled; Emitter* effects[3]; long count;
    void Cancel() { if (!cancelled) { cancelled = true; for (long i = 0; i < count; ++i) { effects[i]->asleep = true; effects[i]->cleared += 2; } } }
    void Resume() { if (cancelled) { cancelled = false; for (long i = 0; i < count; ++i) effects[i]->asleep = false; } }
};
int main()
{
    Emitter e1 = { false, 0 }, e2 = { false, 0 };
    Arrow a = { false, { &e1, &e2, 0 }, 2 };
    a.Cancel(); a.Cancel();
    if (!a.cancelled || !e1.asleep || !e2.asleep || e1.cleared != 2) { std::printf("007cc190_TEST FAIL\n"); return 1; }
    a.Resume(); a.Resume();
    if (a.cancelled || e1.asleep || e2.asleep) { std::printf("007cc190_TEST FAIL\n"); return 1; }
    std::printf("007cc190_TEST PASS\n");
    return 0;
}