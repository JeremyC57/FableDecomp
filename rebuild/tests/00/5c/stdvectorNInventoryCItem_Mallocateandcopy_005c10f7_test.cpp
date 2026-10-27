// behaviour test for 0x005C10F7: the new block holds copies of the range
#include <cstdio>
#include <cstdlib>
#include <new>
struct E { long v[3]; };
static E* AllocateAndCopy(unsigned n, const E* f, const E* l) { E* r = (E*)std::malloc(n * sizeof(E)); E* c = r; for (; f != l; ++f, ++c) new (c) E(*f); return r; }
int main()
{
    E src[2] = { { { 1, 2, 3 } }, { { 4, 5, 6 } } };
    E* r = AllocateAndCopy(4, src, src + 2);
    bool ok = r[0].v[0] == 1 && r[1].v[2] == 6; std::free(r);
    if (!ok) { std::printf("005c10f7_TEST FAIL\n"); return 1; }
    std::printf("005c10f7_TEST PASS\n");
    return 0;
}