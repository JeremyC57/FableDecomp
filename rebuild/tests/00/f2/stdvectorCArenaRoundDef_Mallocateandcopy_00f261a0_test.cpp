#include <cstdio>
#include <cstdlib>
#include <new>
// Behaviour model: n slots allocated (none for 0) and [first, last) copy-constructed into them in order.
struct E { int v; static int copies; E(int x) : v(x) { } E(const E& o) : v(o.v) { ++copies; } };
int E::copies = 0;
static E* AllocCopy(unsigned int n, const E* f, const E* l) { E* r = n ? (E*)std::malloc(n * sizeof(E)) : 0; E* o = r; for (; f != l; ++f, ++o) new (o) E(*f); return r; }
int main()
{
    E src[3] = { E(1), E(2), E(3) };
    E* p = AllocCopy(4, src, src + 3);
    if (!p || p[0].v != 1 || p[2].v != 3 || E::copies != 3 || AllocCopy(0, src, src) != 0) { std::printf("00f261a0_TEST FAIL\n"); return 1; }
    std::free(p);
    std::printf("00f261a0_TEST PASS\n");
    return 0;
}