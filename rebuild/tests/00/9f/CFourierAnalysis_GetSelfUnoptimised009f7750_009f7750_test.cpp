#include <cstdio>
struct X { X* G(long a0) { return this; } };
int main()
{
    X x; if (x.G(0) != &x) { std::printf("009f7750_TEST FAIL\n"); return 1; }

    std::printf("009f7750_TEST PASS\n");
    return 0;
}