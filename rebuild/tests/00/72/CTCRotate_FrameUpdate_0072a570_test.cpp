#include <cstdio>
// Behaviour model of CTCRotate::FrameUpdate (0x0072A570): angle = speed*timer, timer advances, removal past the limit.
struct Rot { bool rotating; float speed; long timer, limit; float lastAngle; int removed; unsigned char flags;
    void Update() { if (rotating) { flags |= 2; lastAngle = speed * timer; if (++timer > limit) ++removed; } } };
int main()
{
    Rot r = { true, 0.5f, 2, 3, 0.0f, 0, 0 };
    r.Update();
    if (r.lastAngle != 1.0f || r.timer != 3 || r.removed || r.flags != 2) { std::printf("0072a570_TEST FAIL\n"); return 1; }
    r.Update();
    if (r.timer != 4 || r.removed != 1) { std::printf("0072a570_TEST FAIL\n"); return 1; }
    std::printf("0072a570_TEST PASS\n");
    return 0;
}