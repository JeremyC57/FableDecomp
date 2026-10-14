#include <cstdio>
struct T { unsigned char p[5]; unsigned char f; }; struct X { T* t; long G() { return t->f & 1; } };
int main()
{
    T t; t.f = 0xff; X x; x.t = &t; if (x.G() != 1) { std::printf("00c8cd00_TEST FAIL\n"); return 1; }

    std::printf("00c8cd00_TEST PASS\n");
    return 0;
}