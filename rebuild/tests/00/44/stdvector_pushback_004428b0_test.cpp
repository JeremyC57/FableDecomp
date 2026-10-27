// behaviour test for 0x004428B0: push_back stores in place while capacity remains and grows otherwise
#include <cstdio>
#include <cstdlib>
#include <cstring>
struct E { unsigned int a, b; };
struct V { E* s; E* f; E* e;
    void grow(const E& x) { unsigned int n = (unsigned int)(f - s), c = n ? 2 * n : 1; E* p = (E*)std::malloc(c * sizeof(E)); if (n) std::memcpy(p, s, n * sizeof(E)); p[n] = x; std::free(s); s = p; f = p + n + 1; e = p + c; }
    void push_back(const E& x) { if (f != e) { *f = x; ++f; } else grow(x); } };
int main()
{
    V v; v.s = v.f = v.e = 0;
    for (unsigned int i = 0; i < 5; ++i) { E x; x.a = i; x.b = 10 * i; v.push_back(x); }
    int ok = v.f - v.s == 5 && v.e - v.s == 8;
    for (unsigned int i = 0; i < 5; ++i) ok = ok && v.s[i].a == i && v.s[i].b == 10 * i;
    if (!ok) { std::printf("004428b0_TEST FAIL\n"); return 1; }
    std::printf("004428b0_TEST PASS\n");
    return 0;
}