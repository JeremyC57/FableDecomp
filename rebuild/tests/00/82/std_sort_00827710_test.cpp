#include <cstdio>
// Behaviour model: the introsort depth limit is 2 * floor(log2(n)); an empty range sorts nothing.
static int lg(int n) { int k = 0; for (; n != 1; n >>= 1) ++k; return k; }
int main()
{
    if (lg(1) * 2 != 0 || lg(2) * 2 != 2 || lg(17) * 2 != 8 || lg(1024) * 2 != 20 || lg(1023) * 2 != 18) { std::printf("00827710_TEST FAIL\n"); return 1; }
    std::printf("00827710_TEST PASS\n");
    return 0;
}