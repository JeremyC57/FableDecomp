// behaviour test for 0x0097C5E0 CAIStateGroup_ScorpionKing::Update() -- phase dispatch: fight phases 0/2/4 (recover from mode 0x38 first, fight, advance when ready), phases 1/3 run phase 2 and queue 2/4, terminate when no longer runnable
#include <cstdio>
struct Boss { long phase; bool ready; long incs; long next; Boss() : phase(0), ready(false), incs(0), next(-1) { } };
struct Group {
    Boss boss; bool hasModes; bool mode38; bool runnable;
    long recovers, combats, phase2s, terminates;
    Group() : hasModes(false), mode38(false), runnable(true), recovers(0), combats(0), phase2s(0), terminates(0) { }
    void Update()
    {
        switch (boss.phase) {
        case 0: case 2: case 4:
            if (hasModes && mode38) ++recovers;
            ++combats;
            if (boss.ready) { ++boss.incs; ++boss.phase; }
            break;
        case 1: ++phase2s; boss.next = 2; break;
        case 3: ++phase2s; boss.next = 4; break;
        }
        if (!runnable) ++terminates;
    }
};
int main()
{
    Group g; g.hasModes = true; g.mode38 = true; g.Update();
    if (g.recovers != 1 || g.combats != 1 || g.boss.incs != 0 || g.terminates != 0) { std::printf("0097c5e0_TEST FAIL\n"); return 1; }
    g.mode38 = false; g.boss.ready = true; g.Update();
    if (g.recovers != 1 || g.combats != 2 || g.boss.phase != 1) { std::printf("0097c5e0_TEST FAIL\n"); return 1; }
    g.Update();
    if (g.phase2s != 1 || g.boss.next != 2 || g.combats != 2) { std::printf("0097c5e0_TEST FAIL\n"); return 1; }
    g.boss.phase = 3; g.runnable = false; g.Update();
    if (g.phase2s != 2 || g.boss.next != 4 || g.terminates != 1) { std::printf("0097c5e0_TEST FAIL\n"); return 1; }
    g.boss.phase = 5; g.Update();
    if (g.phase2s != 2 || g.combats != 2 || g.terminates != 2) { std::printf("0097c5e0_TEST FAIL\n"); return 1; }
    std::printf("0097c5e0_TEST PASS\n");
    return 0;
}