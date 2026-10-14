#include <cstdio>
struct T { unsigned char p[5]; unsigned char f; }; struct X { T* t; long G() { return t->f & 2; } };
int main()
{
    T t; t.f = 0xff; X x; x.t = &t; if (x.G() != 2) { std::printf("00c8cd10_TEST FAIL\n"); return 1; }

    std::printf("00c8cd10_TEST PASS\n");
    return 0;
}