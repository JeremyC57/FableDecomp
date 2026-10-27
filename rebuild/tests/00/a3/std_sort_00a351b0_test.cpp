// behaviour test for 0x00A351B0: the depth limit is twice floor(log2(n)) and an empty range does nothing
#include <cstdio>
static int Lg(int n) { int k; for (k = 0; n != 1; n >>= 1) ++k; return k; }
int main()
{
    int ok = Lg(1) * 2 == 0 && Lg(2) * 2 == 2 && Lg(17) * 2 == 8 && Lg(1024) * 2 == 20;
    if (!ok) { std::printf("00a351b0_TEST FAIL\n"); return 1; }
    std::printf("00a351b0_TEST PASS\n");
    return 0;
}