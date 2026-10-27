// behaviour test for 0x004E9C63 vector<CShopItemDef>::_M_allocate_and_copy (model: a new block holding copies of [first, last))
#include <cstdio>
#include <cstdlib>
#include <new>
static int g_copies = 0;
struct E { int v; E(int x) : v(x) { } E(const E& o) : v(o.v) { ++g_copies; } };
int main()
{
    E src[3] = { E(1), E(2), E(3) };
    unsigned int n = 4;
    E* r = n ? (E*)std::malloc(n * sizeof(E)) : 0; E* cur = r;
    for (const E* p = src; p != src + 3; ++p, ++cur) new (cur) E(*p);
    int ok = r != 0 && r[2].v == 3 && g_copies == 3;
    std::free(r);
    if (!ok) { std::printf("004e9c63_TEST FAIL\n"); return 1; }
    std::printf("004e9c63_TEST PASS\n");
    return 0;
}