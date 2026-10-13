#include <cstdio>
struct O { unsigned char pad[0x10]; float s; }; struct Q { float Query() { return 2.0f; } };
struct X { O* o; Q* q; float F() { return q->Query() * o->s; } };
int main()
{
    O o; o.s = 1.5f; Q q; X x; x.o = &o; x.q = &q; if (x.F() != 3.0f) { std::printf("00c46f40_TEST FAIL\n"); return 1; }

    std::printf("00c46f40_TEST PASS\n");
    return 0;
}