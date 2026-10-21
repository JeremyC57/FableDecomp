#include <cstdio>
#include <math.h>
// Behaviour copy of CAIStateGroup_WhisperPotionCombat::IsRunnableLeaf (0x0096AAD0).
static unsigned long ror13(unsigned long v) { return (v >> 13) | (v << 19); }
static unsigned long WorldRandomRange(unsigned long& seed, unsigned long range)
{
    seed = seed * 0x24A1 + 0x24DF; seed = ror13(seed);
    if (range != 0) return seed % range;
    return 0;
}
struct Group {
    long phase; long frame; long rate; unsigned long seed; long frameLastRun;
    float cx, cy, tx, ty, range; bool jumpOk; int jumpCalls;
    bool RestampAndCheckRange() {
        frameLastRun = frame;
        long r = rate; frameLastRun += WorldRandomRange(seed, r * 5);
        float dx = cx - tx, dy = cy - ty;
        float d = (float)sqrt(dx * dx + dy * dy);
        return d < range;
    }
    bool IsRunnableLeaf() {
        switch (phase) { case 1: return true; case 2: break; default: return false; }
        long delta = frame - frameLastRun;
        if ((float)delta > (float)rate * 10.0f) {
            if (RestampAndCheckRange()) { ++jumpCalls; if (jumpOk) return true; }
        }
        return false;
    }
};
int main()
{
    Group g = { 1, 1000, 15, 7, 0, 0,0, 3,4, 10.0f, true, 0 };
    if (!g.IsRunnableLeaf() || g.jumpCalls != 0) { std::printf("0096aad0_TEST FAIL\n"); return 1; }
    g.phase = 0; if (g.IsRunnableLeaf()) { std::printf("0096aad0_TEST FAIL\n"); return 1; }
    g.phase = 2; g.frameLastRun = 900;             // 100 frames < 150: not yet
    if (g.IsRunnableLeaf() || g.frameLastRun != 900) { std::printf("0096aad0_TEST FAIL\n"); return 1; }
    g.frameLastRun = 0;                             // elapsed: restamp 1000 + rnd(75), dist 5 < 10, jump ok
    unsigned long s = 7; unsigned long expect = 1000 + WorldRandomRange(s, 75);
    if (!g.IsRunnableLeaf() || (unsigned long)g.frameLastRun != expect || g.jumpCalls != 1 || g.seed != s) { std::printf("0096aad0_TEST FAIL\n"); return 1; }
    g.frameLastRun = 0; g.range = 4.0f;             // too far: no jump search
    if (g.IsRunnableLeaf() || g.jumpCalls != 1) { std::printf("0096aad0_TEST FAIL\n"); return 1; }
    g.frameLastRun = 0; g.range = 10.0f; g.jumpOk = false;
    if (g.IsRunnableLeaf() || g.jumpCalls != 2) { std::printf("0096aad0_TEST FAIL\n"); return 1; }
    g.frameLastRun = 0; g.rate = 0; g.jumpOk = true; s = g.seed; unsigned long e2 = 1000 + WorldRandomRange(s, 0);
    if (!g.IsRunnableLeaf() || (unsigned long)g.frameLastRun != e2) { std::printf("0096aad0_TEST FAIL\n"); return 1; }
    std::printf("0096aad0_TEST PASS\n"); return 0;
}