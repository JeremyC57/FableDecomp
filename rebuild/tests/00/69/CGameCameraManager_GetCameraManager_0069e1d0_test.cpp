#include <cstdio>
struct X { bool G(void** o) { *o = this; return true; } };
int main()
{
    X x; void* o = 0; if (!x.G(&o) || o != &x) { std::printf("0069e1d0_TEST FAIL\n"); return 1; }

    std::printf("0069e1d0_TEST PASS\n");
    return 0;
}