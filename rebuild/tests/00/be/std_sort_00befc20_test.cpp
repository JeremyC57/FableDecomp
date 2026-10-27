// behaviour test for 0x00BEFC20: the introsort depth bound is 2 * floor(log2(n))
#include <cstdio>
static int Lg(int n) { int k; for (k = 0; n != 1; n >>= 1) ++k; return k; }
int main()
{
    if (Lg(1) != 0 || Lg(2) != 1 || Lg(7) != 2 || Lg(8) != 3 || 2 * Lg(100) != 12) { std::printf("00befc20_TEST FAIL\n"); return 1; }
    std::printf("00befc20_TEST PASS\n");
    return 0;
}