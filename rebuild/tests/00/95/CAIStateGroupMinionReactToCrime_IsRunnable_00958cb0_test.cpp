#include <cstdio>
// Behaviour copy of CAIStateGroup_MinionReactToCrime::IsRunnable (0x00958CB0).
struct Sim { bool member, village, opinion; bool hasDeed; long type, handle; bool hero, logHas; int removed; bool guard, handled, someoneElse; long stored; bool allowDup; };
static bool IsRunnable(Sim& s) {
    if (s.member && s.village && s.opinion && s.hasDeed) {
        if (s.type == 0x35) {
            if (s.hero && !s.logHas) { ++s.removed; return false; }
            s.stored = s.handle;
        }
        if (s.guard && !s.handled) {
            if (s.type == 7 || s.type == 8) { s.allowDup = true; return true; }
            if (!s.someoneElse) { s.allowDup = false; return true; }
        }
    }
    return false;
}
int main() {
    Sim a = { true, true, true, true, 7, 3, true, true, 0, true, false, true, 0, false };
    if (!IsRunnable(a) || !a.allowDup) { std::printf("00958cb0_TEST FAIL\n"); return 1; }
    Sim b = { true, true, true, true, 0x35, 9, true, false, 0, true, false, false, 0, true };
    if (IsRunnable(b) || b.removed != 1) { std::printf("00958cb0_TEST FAIL\n"); return 1; }
    Sim c = { true, true, true, true, 0x35, 9, true, true, 0, true, false, false, 0, true };
    if (!IsRunnable(c) || c.stored != 9 || c.allowDup) { std::printf("00958cb0_TEST FAIL\n"); return 1; }
    Sim d = c; d.guard = false; if (IsRunnable(d)) { std::printf("00958cb0_TEST FAIL\n"); return 1; }
    Sim e = c; e.someoneElse = true; if (IsRunnable(e)) { std::printf("00958cb0_TEST FAIL\n"); return 1; }
    std::printf("00958cb0_TEST PASS\n"); return 0;
}