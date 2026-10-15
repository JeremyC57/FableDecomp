#include <cstdio>
struct CTCRockTrollShield {
    long health;
    long rechargeCounter;
    void vfunc_9()
    {
        if (rechargeCounter != 0) {
            if (--rechargeCounter == 0) {
                health = 5;
                rechargeCounter = 0;
            }
        }
    }
};
int main()
{
    CTCRockTrollShield s;
    s.health = 1; s.rechargeCounter = 0; s.vfunc_9();
    if (s.health != 1 || s.rechargeCounter != 0) { std::printf("007aed80_TEST FAIL\n"); return 1; }
    s.rechargeCounter = 3; s.vfunc_9();
    if (s.health != 1 || s.rechargeCounter != 2) { std::printf("007aed80_TEST FAIL\n"); return 1; }
    s.vfunc_9(); s.vfunc_9();
    if (s.health != 5 || s.rechargeCounter != 0) { std::printf("007aed80_TEST FAIL\n"); return 1; }
    std::printf("007aed80_TEST PASS\n");
    return 0;
}
