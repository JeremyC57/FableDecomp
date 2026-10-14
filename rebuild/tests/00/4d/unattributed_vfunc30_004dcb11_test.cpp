#include <cstdio>
struct X { bool G(long* o) { *o = 2; return true; } };
int main()
{
    X x; long o = 0; if (!x.G(&o) || o != 2) { std::printf("004dcb11_TEST FAIL\n"); return 1; }

    std::printf("004dcb11_TEST PASS\n");
    return 0;
}