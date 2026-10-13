#include <cstdio>

struct X { unsigned char pad[0x10]; long Total; long Avail;
    void Force(long p) { if (p < 0) p = 0; Avail = p; Total = (Total < Avail) ? Avail : Total; } };
int main()
{
    X x; x.Total = 10; x.Force(-5); if (x.Avail != 0 || x.Total != 10) { std::printf("006d6470_TEST FAIL\n"); return 1; }
    x.Force(25); if (x.Avail != 25 || x.Total != 25) { std::printf("006d6470_TEST FAIL\n"); return 1; }
    std::printf("006d6470_TEST PASS\n");
    return 0;
}