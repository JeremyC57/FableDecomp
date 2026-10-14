#include <cstdio>
struct X { float G(long) { return 0.0f; } };
int main()
{
    X x; if (x.G(0) != 0.0f) { std::printf("006b12f0_TEST FAIL\n"); return 1; }

    std::printf("006b12f0_TEST PASS\n");
    return 0;
}