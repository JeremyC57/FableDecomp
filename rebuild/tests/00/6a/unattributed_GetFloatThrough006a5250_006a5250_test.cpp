#include <cstdio>
struct T { unsigned char p[4]; float v; }; struct X { unsigned char p[4]; T* t; float Get() { return t->v; } };
int main()
{
    T t; t.v = 1.5f; X x; x.t = &t; if (x.Get() != 1.5f) { std::printf("006a5250_TEST FAIL\n"); return 1; }

    std::printf("006a5250_TEST PASS\n");
    return 0;
}