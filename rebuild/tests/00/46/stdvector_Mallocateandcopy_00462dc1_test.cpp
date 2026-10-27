// behaviour test for 0x00462DC1: the new block holds a copy of every element
#include <cstdio>
#include <cstdlib>
struct E { unsigned int a[3]; };
static E* AllocateAndCopy(unsigned int n, const E* first, const E* last)
{
    E* r = n ? (E*)std::malloc(n * sizeof(E)) : 0; E* c = r;
    for (; first != last; ++first, ++c) if (c) *c = *first;
    return r;
}
int main()
{
    E v[3]; for (unsigned int i = 0; i < 3; ++i) for (int j = 0; j < 3; ++j) v[i].a[j] = 10 * i + j;
    E* p = AllocateAndCopy(4, v, v + 3); E* z = AllocateAndCopy(0, v, v);
    if (!p || p[0].a[0] != 0 || p[2].a[3 - 1] != 20 + 3 - 1 || z) { std::printf("00462dc1_TEST FAIL\n"); return 1; }
    std::free(p);
    std::printf("00462dc1_TEST PASS\n");
    return 0;
}