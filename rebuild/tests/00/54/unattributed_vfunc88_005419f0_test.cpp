#include <cstdio>
struct X { float G(long, long) { return 0.0f; } };
int main()
{
    X x; if (x.G(0, 0) != 0.0f) { std::printf("005419f0_TEST FAIL\n"); return 1; }

    std::printf("005419f0_TEST PASS\n");
    return 0;
}