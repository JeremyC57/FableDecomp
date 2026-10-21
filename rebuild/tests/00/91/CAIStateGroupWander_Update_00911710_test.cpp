#include <cstdio>
// Behaviour copy of CAIStateGroup_Wander::Update (0x00911710): wander 5m, then with probability 1/2 idle Random(2) times.
static unsigned long ror13(unsigned long v) { return (v >> 13) | (v << 19); }
struct Group {
    unsigned long seed; bool wanderOk; long moveResult; int idles; int idleFailAt; int moves;
    unsigned long Random(unsigned long range) { seed = seed * 0x24A1U + 0x24DFU; seed = ror13(seed); return seed % range; }
    long PerformIdle() { ++idles; return idles == idleFailAt ? 1 : 2; }
    void Update() {
        if (!wanderOk) return;
        ++moves; if (moveResult != 2) return;
        if (Random(100) >= 50) return;
        unsigned long count = Random(2);
        if (count == 0) return;
        for (unsigned long i = 0; i < count; ++i) { bool failed = PerformIdle() != 2; if (failed) return; }
    }
};
static unsigned long step(unsigned long& s) { s = s * 0x24A1U + 0x24DFU; s = ror13(s); return s; }
int main() {
    // find a seed whose first roll is < 50 and second is odd, and one whose first roll is >= 50
    unsigned long seedIdle = 0, seedNoIdle = 0, seedSkip = 0;
    for (unsigned long c = 1; c < 100000 && (!seedIdle || !seedNoIdle || !seedSkip); ++c) {
        unsigned long s = c; unsigned long r1 = step(s) % 100; unsigned long r2 = step(s) % 2;
        if (r1 >= 50) { if (!seedSkip) seedSkip = c; }
        else if (r2 == 1) { if (!seedIdle) seedIdle = c; }
        else if (!seedNoIdle) seedNoIdle = c;
    }
    if (!seedIdle || !seedNoIdle || !seedSkip) { std::printf("00911710_TEST FAIL\n"); return 1; }
    Group g = { seedIdle, true, 2, 0, -1, 0 }; g.Update();
    if (g.idles != 1 || g.moves != 1) { std::printf("00911710_TEST FAIL\n"); return 1; }
    Group n = { seedNoIdle, true, 2, 0, -1, 0 }; n.Update();
    Group k = { seedSkip, true, 2, 0, -1, 0 }; k.Update();
    Group h = { seedIdle, false, 2, 0, -1, 0 }; h.Update();
    Group f = { seedIdle, true, 4, 0, -1, 0 }; f.Update();
    if (n.idles != 0 || k.idles != 0 || h.moves != 0 || f.idles != 0 || f.moves != 1) { std::printf("00911710_TEST FAIL\n"); return 1; }
    std::printf("00911710_TEST PASS\n"); return 0;
}