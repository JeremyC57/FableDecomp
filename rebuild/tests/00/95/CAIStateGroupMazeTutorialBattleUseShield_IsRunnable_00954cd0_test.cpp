#include <cstdio>
// Behaviour copy of CAIStateGroup_MazeTutorialBattleUseShield::IsRunnable (0x00954CD0).
struct Target { bool dead; };
struct State { long phase; Target* target; long lastShield; long latched; int defFetches; };
static bool IsRunnable(State& s, long frame, long fps) {
    if (s.phase == 2) {
        Target* t = s.target;
        if (t && !t->dead) {
            float elapsed = (float)(frame - s.lastShield);
            if (elapsed > (float)fps * 8.0f) {
                s.lastShield = frame;
                ++s.defFetches;
                s.latched = 1;
                return true;
            }
        }
    }
    return false;
}
int main() {
    Target alive = { false }, dead = { true };
    State s = { 2, &alive, 0, 0, 0 };
    if (IsRunnable(s, 120, 15)) { std::printf("00954cd0_TEST FAIL\n"); return 1; }
    if (!IsRunnable(s, 121, 15) || s.lastShield != 121 || s.latched != 1 || s.defFetches != 1) { std::printf("00954cd0_TEST FAIL\n"); return 1; }
    if (IsRunnable(s, 200, 15)) { std::printf("00954cd0_TEST FAIL\n"); return 1; }
    State d = { 2, &dead, 0, 0, 0 }; if (IsRunnable(d, 1000, 15)) { std::printf("00954cd0_TEST FAIL\n"); return 1; }
    State p = { 3, &alive, 0, 0, 0 }; if (IsRunnable(p, 1000, 15)) { std::printf("00954cd0_TEST FAIL\n"); return 1; }
    std::printf("00954cd0_TEST PASS\n"); return 0;
}