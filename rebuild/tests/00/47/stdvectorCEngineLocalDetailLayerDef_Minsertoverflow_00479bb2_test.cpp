// behaviour test for 0x00479BB2 vector<CEngineLocalDetailLayerDef>::_M_insert_overflow (model: head, n copies, tail copy-constructed into a grown block)
#include <cstdio>
#include <cstdlib>
#include <new>
static int g_copies = 0;
struct E { int v; E(int x) : v(x) { } E(const E& o) : v(o.v) { ++g_copies; } };
int main()
{
    E* old = (E*)std::malloc(3 * sizeof(E));
    for (int i = 0; i < 3; ++i) new (old + i) E(i + 1);
    E x(9);
    unsigned int oldSize = 3, n = 1, len = oldSize + (oldSize > n ? oldSize : n);
    E* nb = (E*)std::malloc(len * sizeof(E)); E* f = nb;
    for (E* p = old; p != old + 2; ++p) new (f++) E(*p);
    new (f++) E(x);
    for (E* p = old + 2; p != old + 3; ++p) new (f++) E(*p);
    int ok = len == 6 && f - nb == 4 && nb[2].v == 9 && nb[3].v == 3 && g_copies == 4;
    std::free(nb); std::free(old);
    if (!ok) { std::printf("00479bb2_TEST FAIL\n"); return 1; }
    std::printf("00479bb2_TEST PASS\n");
    return 0;
}