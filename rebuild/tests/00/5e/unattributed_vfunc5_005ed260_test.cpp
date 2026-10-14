#include <cstdio>
struct X { bool G(long* o) { *o = 1045220557; return true; } };
int main()
{
    X x; long o = 0; if (!x.G(&o) || o != 1045220557) { std::printf("005ed260_TEST FAIL\n"); return 1; }

    std::printf("005ed260_TEST PASS\n");
    return 0;
}