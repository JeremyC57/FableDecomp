#include <cstdio>
// Behaviour copy of CAIStateGroup_TavernKeeper::WalkToPoint (0x00968230).
struct V { float X, Y, Z; };
struct Sim { bool pub, bar; long moves[4]; int mi; long looks[4]; int li; V lastLook; };
static bool WalkToPoint(Sim& s, const V& point, const V& forward, const V& pos) {
    long r;
    do { if (!s.pub || !s.bar) return false; r = s.moves[s.mi++]; } while (r == 4);
    if (r != 2) return false;
    V look = { forward.X + pos.X, forward.Y + pos.Y, forward.Z + pos.Z };
    do { if (!s.pub || !s.bar) return false; s.lastLook = look; r = s.looks[s.li++]; } while (r == 4);
    return r == 2 ? true : false;
}
int main() {
    V p = { 1, 2, 3 }, f = { 0, 1, 0 }, c = { 5, 5, 0 };
    Sim a = { true, true, { 4, 4, 2, 0 }, 0, { 4, 2, 0, 0 }, 0, { 0, 0, 0 } };
    if (!WalkToPoint(a, p, f, c) || a.mi != 3 || a.li != 2 || a.lastLook.Y != 6.0f) { std::printf("00968230_TEST FAIL\n"); return 1; }
    Sim b = { true, true, { 3, 0, 0, 0 }, 0, { 2, 0, 0, 0 }, 0, { 0, 0, 0 } };
    if (WalkToPoint(b, p, f, c) || b.li != 0) { std::printf("00968230_TEST FAIL\n"); return 1; }
    Sim d = { true, false, { 2, 0, 0, 0 }, 0, { 2, 0, 0, 0 }, 0, { 0, 0, 0 } };
    if (WalkToPoint(d, p, f, c) || d.mi != 0) { std::printf("00968230_TEST FAIL\n"); return 1; }
    std::printf("00968230_TEST PASS\n"); return 0;
}