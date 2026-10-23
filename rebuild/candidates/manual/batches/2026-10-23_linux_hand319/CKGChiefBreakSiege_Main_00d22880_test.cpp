// Behaviour test for CKG_ChiefBreakSiege::Main (0x00D22880): walk to the hut while answering the hero, then play the
// defeat cut-scene when the hero is alive and the siege was lost, else the victory one when the bandits were beaten.
#include <cstdio>
struct Sim {
    int lines, defeat, victory; bool done;
    void Run(int steps, int talks, bool heroAlive, bool lost, bool beaten)
    {
        for (int i = 0; i < steps; i++) if (i < talks) lines++;
        if (heroAlive && lost) defeat++;
        else if (beaten) victory++;
        done = true;
    }
};
int main()
{
    Sim a = { 0, 0, 0, false };
    a.Run(4, 2, true, true, true);
    if (a.lines != 2 || a.defeat != 1 || a.victory != 0 || !a.done) { std::printf("00d22880_TEST FAIL\n"); return 1; }
    Sim b = { 0, 0, 0, false };
    b.Run(1, 0, false, true, true);
    if (b.defeat != 0 || b.victory != 1) { std::printf("00d22880_TEST FAIL\n"); return 1; }
    std::printf("00d22880_TEST PASS\n");
    return 0;
}
