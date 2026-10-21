#include <cstdio>
// Behaviour copy of CAIStateGroup_ChickenWander::Update (0x009118A0): wander, then 1..3 idles from the world seed.
static unsigned long ror13(unsigned long v) { return (v >> 13) | (v << 19); }
struct Group {
    unsigned long seed; bool wanderOk; long moveResult; int idles; int idleFailAt; int moves;
    unsigned long Random(unsigned long range) { unsigned long next = seed * 0x24A1U + 0x24DFU; seed = next; seed = ror13(next); return seed % range; }
    long PerformIdle() { ++idles; return idles == idleFailAt ? 1 : 2; }
    void Update() {
        if (!wanderOk) return;
        ++moves; if (moveResult != 2) return;
        unsigned long count = Random(3) + 1;
        for (unsigned long i = 0; i < count; ++i) { bool failed = PerformIdle() != 2; if (failed) return; }
    }
};
int main() {
    Group g = { 12345, true, 2, 0, -1, 0 };
    unsigned long s = 12345 * 0x24A1U + 0x24DFU; s = ror13(s); unsigned long expect = s % 3 + 1;
    g.Update();
    if ((unsigned long)g.idles != expect || g.moves != 1) { std::printf("009118a0_TEST FAIL\n"); return 1; }
    Group h = { 1, false, 2, 0, -1, 0 }; h.Update();
    Group k = { 1, true, 4, 0, -1, 0 }; k.Update();
    Group m = { 12345, true, 2, 0, 1, 0 }; m.Update();
    if (h.moves != 0 || k.idles != 0 || m.idles != 1) { std::printf("009118a0_TEST FAIL\n"); return 1; }
    std::printf("009118a0_TEST PASS\n"); return 0;
}
