#include <cstdio>
struct T { unsigned char p[4]; unsigned char f; }; struct X { unsigned char q[4]; T* t; unsigned char G() { return t->f & 1; } };
int main()
{
    T t; t.f = 0xff; X x; x.t = &t; if (x.G() != 1) { std::printf("006622c0_TEST FAIL\n"); return 1; }

    std::printf("006622c0_TEST PASS\n");
    return 0;
}