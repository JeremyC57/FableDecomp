#include <cstdio>
struct T { unsigned char p[4]; float v; }; struct X { unsigned char p[4]; T* t; float G(long) { return t->v; } };
int main()
{
    T t; t.v = 2.5f; X x; x.t = &t; if (x.G(0) != 2.5f) { std::printf("006a9c40_TEST FAIL\n"); return 1; }

    std::printf("006a9c40_TEST PASS\n");
    return 0;
}