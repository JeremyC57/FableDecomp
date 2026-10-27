// behaviour test for 0x008ECF60: __find_if returns the first matching pair or last (all remainder cases)
#include <cstdio>
struct P { long k; long v; };
static P* FindIf(P* f, P* l, long key) { for (; f != l; ++f) if (f->k == key) return f; return l; }
int main()
{
    P a[7]; for (int i = 0; i < 7; ++i) { a[i].k = i * 10; a[i].v = 0; }
    int ok = 1;
    for (int n = 0; n <= 7; ++n) for (int i = 0; i < 7; ++i) { P* r = FindIf(a, a + n, i * 10); if ((i < n) ? r != a + i : r != a + n) ok = 0; }
    if (!ok) { std::printf("008ecf60_TEST FAIL\n"); return 1; }
    std::printf("008ecf60_TEST PASS\n");
    return 0;
}