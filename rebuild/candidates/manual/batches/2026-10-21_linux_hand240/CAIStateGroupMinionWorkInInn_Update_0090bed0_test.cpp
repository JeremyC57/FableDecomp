#include <cstdio>
// Behaviour copy of CAIStateGroup_MinionWorkInInn::Update (0x0090BED0).
struct Sim { bool workplace; unsigned long seed; int cleaned, idles, terminated, moves; long moveResults[4]; int mi; };
static unsigned long Ror13(unsigned long v) { return (v >> 13) | (v << 19); }
static unsigned long Random(Sim& s, unsigned long range) { s.seed = s.seed * 0x24A1U + 0x24DFU; s.seed = Ror13(s.seed); return s.seed % range; }
static void Update(Sim& s) {
    if (!s.workplace) { ++s.terminated; return; }
    if (Random(s, 100) < 21) { ++s.cleaned; return; }
    long r;
    do { if (!s.workplace) { ++s.terminated; return; } ++s.moves; r = s.moveResults[s.mi++]; } while (r == 4);
    if (r == 2 && s.workplace) { ++s.idles; return; }
    ++s.terminated;
}
int main() {
    int cleaned = 0, walked = 0;
    for (unsigned long seed = 1; seed < 200; ++seed) {
        Sim s = { true, seed, 0, 0, 0, 0, { 4, 2, 0, 0 }, 0 };
        Update(s);
        if (s.cleaned) ++cleaned; else { if (s.moves != 2 || s.idles != 1 || s.terminated) { std::printf("0090bed0_TEST FAIL\n"); return 1; } ++walked; }
    }
    if (cleaned == 0 || walked == 0) { std::printf("0090bed0_TEST FAIL\n"); return 1; }
    Sim n = { false, 1, 0, 0, 0, 0, { 2 }, 0 }; Update(n); if (n.terminated != 1) { std::printf("0090bed0_TEST FAIL\n"); return 1; }
    std::printf("0090bed0_TEST PASS\n"); return 0;
}
