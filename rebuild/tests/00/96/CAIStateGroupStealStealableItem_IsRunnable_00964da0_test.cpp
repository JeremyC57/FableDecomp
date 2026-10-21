#include <cstdio>
// Behaviour copy of CAIStateGroup_StealStealableItem::IsRunnable (0x00964DA0).
struct Sim { bool cleanup, canSteal, anywhere; int item, escape; bool itemAlive, escapeAlive; long ap, pick; bool got, reached; int latchedItem, latchedEscape; bool lastAnywhereArg; };
static bool IsRunnable(Sim& s) {
    if (!s.cleanup) {
        if (s.canSteal) {
            s.ap = -1; s.reached = false; s.got = false; s.pick = -1;
            s.latchedItem = s.item;
            s.lastAnywhereArg = !s.anywhere; s.latchedEscape = s.escape;
            return (s.latchedItem && s.itemAlive) && (s.latchedEscape && s.escapeAlive);
        }
    }
    return false;
}
int main() {
    Sim a = { false, true, false, 3, 4, true, true, 7, 7, true, true, 0, 0, false };
    if (!IsRunnable(a) || a.ap != -1 || a.pick != -1 || a.got || a.reached || !a.lastAnywhereArg) { std::printf("00964da0_TEST FAIL\n"); return 1; }
    Sim b = a; b.escape = 0; if (IsRunnable(b)) { std::printf("00964da0_TEST FAIL\n"); return 1; }
    Sim c = a; c.cleanup = true; c.ap = 7; if (IsRunnable(c) || c.ap != 7) { std::printf("00964da0_TEST FAIL\n"); return 1; }
    Sim d = a; d.canSteal = false; d.ap = 7; if (IsRunnable(d) || d.ap != 7) { std::printf("00964da0_TEST FAIL\n"); return 1; }
    std::printf("00964da0_TEST PASS\n"); return 0;
}