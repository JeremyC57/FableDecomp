#include <cstdio>
struct S { unsigned char p[4]; void* v; }; struct Fi { unsigned char p[8]; S* s; }; struct X { unsigned char p[4]; Fi* f; void* G() { return f->s->v; } };
int main()
{
    int i; S s; s.v = &i; Fi f; f.s = &s; X x; x.f = &f; if (x.G() != &i) { std::printf("008fcfa0_TEST FAIL\n"); return 1; }

    std::printf("008fcfa0_TEST PASS\n");
    return 0;
}