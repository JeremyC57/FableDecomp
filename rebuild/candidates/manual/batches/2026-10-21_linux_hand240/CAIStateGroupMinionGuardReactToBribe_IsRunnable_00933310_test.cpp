#include <cstdio>
// Behaviour copy of CAIStateGroup_MinionGuardReactToBribe::IsRunnable (0x00933310).
struct Sim { bool hasTC; long pending; long offered; long lastReacted; bool heroAlive; int hero; bool received; };
static bool IsRunnable(Sim& s, long now, long fps) {
    if (s.hasTC) {
        if (s.pending < 0) {
            s.hero = 1;
            if (s.heroAlive) { s.received = false; return true; }
        }
        long deadline = s.offered + fps * 4;
        long earliest = fps * 7 + s.lastReacted;
        if (now < deadline && deadline > earliest) {
            s.hero = 1;
            if (s.heroAlive) { s.received = true; return true; }
        }
    }
    return false;
}
int main() {
    Sim a = { true, -1, 0, 0, true, 0, true };
    if (!IsRunnable(a, 10, 15) || a.received || a.hero != 1) { std::printf("00933310_TEST FAIL\n"); return 1; }
    Sim b = { true, 0, 200, 50, true, 0, false };
    if (!IsRunnable(b, 210, 15) || !b.received) { std::printf("00933310_TEST FAIL\n"); return 1; }   // deadline 260 > earliest 155
    Sim c = { true, 0, 100, 50, true, 0, false };
    if (IsRunnable(c, 210, 15)) { std::printf("00933310_TEST FAIL\n"); return 1; }    // now past deadline 160
    Sim d = { false, -1, 0, 0, true, 0, false };
    if (IsRunnable(d, 0, 15)) { std::printf("00933310_TEST FAIL\n"); return 1; }
    std::printf("00933310_TEST PASS\n"); return 0;
}
