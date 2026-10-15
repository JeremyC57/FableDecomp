#include <cstdio>
struct CTCWeapon {
    long AimingTime; long TimeWielded; bool Wielded;
    void FrameUpdate() { if (Wielded) ++TimeWielded; if (AimingTime != -1) ++AimingTime; }
};
int main()
{
    CTCWeapon w; w.AimingTime = -1; w.TimeWielded = 0; w.Wielded = false;
    w.FrameUpdate();
    if (w.AimingTime != -1 || w.TimeWielded != 0) { std::printf("005da4f0_TEST FAIL\n"); return 1; }
    w.Wielded = true; w.AimingTime = 0; w.FrameUpdate(); w.FrameUpdate();
    if (w.AimingTime != 2 || w.TimeWielded != 2) { std::printf("005da4f0_TEST FAIL\n"); return 1; }
    std::printf("005da4f0_TEST PASS\n");
    return 0;
}
