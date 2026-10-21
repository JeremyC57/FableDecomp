#include <cstdio>
// Behaviour copy of CAIStateGroup_SeekEnemy::IsRunnable (0x0093B740).
struct Sim { int enemy; bool dead, willing, hasPerceive; int perceived; bool aware; long frames; bool canSee; int latched; };
static bool IsRunnable(Sim& s, long fps) {
    int enemy = s.enemy;
    if (enemy && !s.dead && s.willing) {
        if (s.hasPerceive) {
            if (s.perceived == enemy) {
                if (s.aware) {
                    long frames = s.frames;
                    if (frames != -1 && (float)frames < (float)fps * 15.0f) { s.latched = enemy; return true; }
                }
                return false;
            }
        }
        if (s.canSee) { s.latched = enemy; return true; }
    }
    return false;
}
int main() {
    Sim a = { 4, false, true, true, 4, true, 100, false, 0 };
    if (!IsRunnable(a, 15) || a.latched != 4) { std::printf("0093b740_TEST FAIL\n"); return 1; }
    Sim b = a; b.frames = 300; b.latched = 0; if (IsRunnable(b, 15) || b.latched) { std::printf("0093b740_TEST FAIL\n"); return 1; }
    Sim c = a; c.frames = -1; if (IsRunnable(c, 15)) { std::printf("0093b740_TEST FAIL\n"); return 1; }
    Sim d = { 4, false, true, true, 9, true, 100, true, 0 };
    if (!IsRunnable(d, 15) || d.latched != 4) { std::printf("0093b740_TEST FAIL\n"); return 1; }
    Sim e = d; e.canSee = false; e.latched = 0; if (IsRunnable(e, 15)) { std::printf("0093b740_TEST FAIL\n"); return 1; }
    Sim f = d; f.dead = true; if (IsRunnable(f, 15)) { std::printf("0093b740_TEST FAIL\n"); return 1; }
    std::printf("0093b740_TEST PASS\n"); return 0;
}
