#include <cstdio>
#include <cstdlib>
static void* __cdecl H(void*, void** a, void**, void**) { return *a; }
struct T { unsigned char p[4]; void* a; void* b; void* c; }; struct X { T* t; long n; void R(void* k) { void* f = H(k, &t->a, &t->b, &t->c); if (f) std::free(f); --n; } };
int main()
{
    T t; t.a = std::malloc(4); X x; x.t = &t; x.n = 2; x.R(0); if (x.n != 1) { std::printf("00990070_TEST FAIL\n"); return 1; }

    std::printf("00990070_TEST PASS\n");
    return 0;
}