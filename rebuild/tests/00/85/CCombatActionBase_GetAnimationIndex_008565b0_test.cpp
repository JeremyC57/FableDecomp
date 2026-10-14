#include <cstdio>
struct T { void* f; }; struct X { unsigned char p[4]; T* t; void* G() { return t->f; } };
int main()
{
    int i; T t; t.f = &i; X x; x.t = &t; if (x.G() != &i) { std::printf("008565b0_TEST FAIL\n"); return 1; }

    std::printf("008565b0_TEST PASS\n");
    return 0;
}