// behaviour test for 0x0085DE40 the 4-byte vector copy (model: same contents, own storage)
#include <cstdio>
#include <cstdlib>
#include <new>
struct V { int* s; int* f; int* e; };
static void Copy(V& d, const V& x)
{
    unsigned int n = (unsigned int)(x.f - x.s);
    d.s = n ? (int*)std::malloc(n * sizeof(int)) : 0; d.f = d.s; d.e = d.s + n;
    int* cur = d.s; for (const int* p = x.s; p != x.f; ++p, ++cur) new (cur) int(*p);
    d.f = cur;
}
int main()
{
    int a[3] = { 4, 5, 6 }; V x = { a, a + 3, a + 3 }, d, e0;
    Copy(d, x);
    V empty = { 0, 0, 0 }; Copy(e0, empty);
    int ok = d.s != a && d.f == d.s + 3 && d.e == d.s + 3 && d.s[0] == 4 && d.s[2] == 6 && e0.s == 0 && e0.f == 0;
    std::free(d.s);
    if (!ok) { std::printf("0085de40_TEST FAIL\n"); return 1; }
    std::printf("0085de40_TEST PASS\n");
    return 0;
}