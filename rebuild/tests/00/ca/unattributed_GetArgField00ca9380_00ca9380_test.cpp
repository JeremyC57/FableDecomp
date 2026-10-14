#include <cstdio>
struct T { unsigned char p[4]; void* v; }; struct X { void* G(const T* t) { return t->v; } };
int main()
{
    int i; T t; t.v = &i; X x; if (x.G(&t) != &i) { std::printf("00ca9380_TEST FAIL\n"); return 1; }

    std::printf("00ca9380_TEST PASS\n");
    return 0;
}