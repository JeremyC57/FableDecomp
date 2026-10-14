#include <cstdio>
struct T { unsigned char p[4]; unsigned char v; }; struct X { unsigned char p[4]; T* t; unsigned char G() { return t->v; } };
int main()
{
    T t; t.v = 9; X x; x.t = &t; if (x.G() != 9) { std::printf("00815270_TEST FAIL\n"); return 1; }

    std::printf("00815270_TEST PASS\n");
    return 0;
}