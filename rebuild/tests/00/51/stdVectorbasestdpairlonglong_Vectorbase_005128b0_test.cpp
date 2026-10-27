// behaviour test for 0x005128B0 _Vector_base(n, a) (model: start = finish = malloc(n * size), end = start + n)
#include <cstdio>
#include <cstdlib>
struct B { int* s; int* f; int* e; B(unsigned int n) : s(0), f(0), e(0) { s = n ? (int*)std::malloc(n * sizeof(int)) : 0; f = s; e = s + n; } };
int main()
{
    B b(5); B z(0);
    int ok = b.s != 0 && b.f == b.s && b.e == b.s + 5 && z.s == 0 && z.e == 0;
    std::free(b.s);
    if (!ok) { std::printf("005128b0_TEST FAIL\n"); return 1; }
    std::printf("005128b0_TEST PASS\n");
    return 0;
}