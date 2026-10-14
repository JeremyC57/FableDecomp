#include <cstdio>
struct T { unsigned char p[5]; unsigned char f; }; struct X { T* t; long G() { return t->f & 4; } };
int main()
{
    T t; t.f = 0xff; X x; x.t = &t; if (x.G() != 4) { std::printf("00c8cd20_TEST FAIL\n"); return 1; }

    std::printf("00c8cd20_TEST PASS\n");
    return 0;
}