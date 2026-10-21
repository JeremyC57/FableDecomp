#include <cstdio>
// Behaviour copy of CAIStateGroup_TavernServer::IsRunnable (0x00967650).
struct Sim { bool carrying, member, village; int pub, workplace; bool pubAlive, barAlive, barFound; long serving; bool servingFound, jug; int freedServing, freedBar; int jugPtr; };
static bool IsRunnable(Sim& s) {
    if (s.carrying) return true;
    if (s.member) {
        if (!s.pub) s.pub = s.workplace;
        if (s.pub && s.pubAlive && s.village && (s.barAlive || s.barFound) && (s.serving != -1 || s.servingFound)) {
            ++s.freedServing;
            if (s.jug) { ++s.freedBar; s.jugPtr = 0; return true; }
        }
    }
    return false;
}
int main() {
    Sim a = { false, true, true, 0, 5, true, false, true, -1, true, true, 0, 0, 9 };
    if (!IsRunnable(a) || a.pub != 5 || a.freedServing != 1 || a.freedBar != 1 || a.jugPtr != 0) { std::printf("00967650_TEST FAIL\n"); return 1; }
    Sim b = a; b.carrying = true; b.freedServing = 0; if (!IsRunnable(b) || b.freedServing != 0) { std::printf("00967650_TEST FAIL\n"); return 1; }
    Sim c = { false, true, true, 0, 0, true, true, true, 3, true, true, 0, 0, 9 };
    if (IsRunnable(c) || c.jugPtr != 9) { std::printf("00967650_TEST FAIL\n"); return 1; }
    Sim d = { false, true, true, 2, 5, true, true, true, 3, true, false, 0, 0, 9 };
    if (IsRunnable(d) || d.freedServing != 1 || d.freedBar != 0) { std::printf("00967650_TEST FAIL\n"); return 1; }
    std::printf("00967650_TEST PASS\n"); return 0;
}