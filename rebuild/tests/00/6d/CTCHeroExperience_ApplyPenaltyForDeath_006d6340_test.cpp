#include <cstdio>

struct X { unsigned char pad[0x10]; long Total; long Avail;
    void Apply() { long p = Avail / 2; Avail -= p; Total -= p; } };
int main()
{
    X x; x.Total = 100; x.Avail = 31; x.Apply();
    if (x.Avail != 16 || x.Total != 85) { std::printf("006d6340_TEST FAIL\n"); return 1; }
    std::printf("006d6340_TEST PASS\n");
    return 0;
}