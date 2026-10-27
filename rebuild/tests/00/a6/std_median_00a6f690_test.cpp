#include <cstdio>
// Behaviour model: the median of three keys.
static int med(int a, int b, int c)
{
    if (a < b) { if (b < c) return b; else if (a < c) return c; else return a; }
    else if (a < c) return a; else if (b < c) return c; else return b;
}
int main()
{
    if (med(1, 2, 3) != 2 || med(3, 1, 2) != 2 || med(2, 3, 1) != 2 || med(5, 5, 1) != 5) { std::printf("00a6f690_TEST FAIL\n"); return 1; }
    std::printf("00a6f690_TEST PASS\n");
    return 0;
}