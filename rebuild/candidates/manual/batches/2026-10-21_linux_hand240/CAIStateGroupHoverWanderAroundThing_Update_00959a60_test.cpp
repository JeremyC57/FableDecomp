#include <cstdio>
// Behaviour copy of CAIStateGroup_HoverWanderAroundThing::Update (0x00959A60).
struct Sim { bool alive; bool wanderOk; long lookResult; long moveResults[8]; int moveIdx; int moves; int terminated; unsigned long seed; long lastMovement; };
static unsigned long Ror13(unsigned long v) { return (v >> 13) | (v << 19); }
static unsigned long Random(Sim& s, unsigned long range) { s.seed = s.seed * 0x24A1U + 0x24DFU; s.seed = Ror13(s.seed); return s.seed % range; }
static void Update(Sim& s) {
    for (long i = 0; i != 3; ++i) {
        if (s.alive && s.wanderOk) {
            if (s.lookResult != 2) { ++s.terminated; return; }
            long movement = (long)Random(s, 3) + 8;
            long r;
            do {
                if (!s.alive) { ++s.terminated; return; }
                r = s.moveResults[s.moveIdx++]; ++s.moves; s.lastMovement = movement;
            } while (r == 4);
            if (r != 2) { ++s.terminated; return; }
        }
    }
}
int main() {
    Sim s = { true, true, 2, { 4, 2, 2, 4, 4, 2, 0, 0 }, 0, 0, 0, 1, 0 };
    Update(s);
    if (s.moves != 6 || s.terminated != 0 || s.lastMovement < 8 || s.lastMovement > 10) { std::printf("00959a60_TEST FAIL\n"); return 1; }
    Sim t = { true, true, 1, { 2 }, 0, 0, 0, 1, 0 };
    Update(t); if (t.terminated != 1 || t.moves != 0) { std::printf("00959a60_TEST FAIL\n"); return 1; }
    Sim u = { false, true, 2, { 2 }, 0, 0, 0, 1, 0 };
    Update(u); if (u.terminated != 0 || u.moves != 0) { std::printf("00959a60_TEST FAIL\n"); return 1; }
    Sim v = { true, true, 2, { 4, 3 }, 0, 0, 0, 1, 0 };
    Update(v); if (v.terminated != 1 || v.moves != 2) { std::printf("00959a60_TEST FAIL\n"); return 1; }
    std::printf("00959a60_TEST PASS\n"); return 0;
}
