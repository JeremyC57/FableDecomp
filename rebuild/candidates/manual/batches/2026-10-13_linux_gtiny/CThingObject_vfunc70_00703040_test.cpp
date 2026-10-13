#include <cstdio>
struct T { unsigned char p[4]; long v; }; struct X { unsigned char p[4]; T* t; long Get() { return t->v; } };
int main()
{
    T t; t.v = 5; X x; x.t = &t; if (x.Get() != 5) { std::printf("00703040_TEST FAIL\n"); return 1; }

    std::printf("00703040_TEST PASS\n");
    return 0;
}
