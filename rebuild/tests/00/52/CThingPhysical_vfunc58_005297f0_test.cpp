#include <cstdio>
struct X { void G(long* o) { *o = 0; } };
int main()
{
    X x; long o = 5; x.G(&o); if (o != 0) { std::printf("005297f0_TEST FAIL\n"); return 1; }

    std::printf("005297f0_TEST PASS\n");
    return 0;
}