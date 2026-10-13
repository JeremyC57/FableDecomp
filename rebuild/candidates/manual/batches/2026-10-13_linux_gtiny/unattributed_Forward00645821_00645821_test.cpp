#include <cstdio>
struct O { long M() { return 4; } }; struct X { unsigned char p[4]; O* o; long F() { return o->M(); } };
int main()
{
    O o; X x; x.o = &o; if (x.F() != 4) { std::printf("00645821_TEST FAIL\n"); return 1; }

    std::printf("00645821_TEST PASS\n");
    return 0;
}
