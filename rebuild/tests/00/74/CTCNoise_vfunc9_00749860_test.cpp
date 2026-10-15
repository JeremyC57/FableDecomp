// Behaviour test for CTCNoise::vfunc_9 @ 0x00749860: timer counts down, kill exactly when it reaches zero.
#include <cstdio>
struct CThing { int kills; bool arg; void Kill(bool removeNow) { ++kills; arg = removeNow; } };
struct CTCNoise {
    CThing* ParentThing;
    long LifeTimer;
    void vfunc_9() { if (LifeTimer > 0) --LifeTimer; if (LifeTimer == 0) ParentThing->Kill(true); }
};
int main()
{
    CThing t; t.kills = 0; t.arg = false;
    CTCNoise n; n.ParentThing = &t; n.LifeTimer = 2;
    n.vfunc_9();
    if (n.LifeTimer != 1 || t.kills != 0) { std::printf("00749860_TEST FAIL\n"); return 1; }
    n.vfunc_9();
    if (n.LifeTimer != 0 || t.kills != 1 || !t.arg) { std::printf("00749860_TEST FAIL\n"); return 1; }
    n.LifeTimer = -1; n.vfunc_9();
    if (n.LifeTimer != -1 || t.kills != 1) { std::printf("00749860_TEST FAIL\n"); return 1; }
    std::printf("00749860_TEST PASS\n");
    return 0;
}