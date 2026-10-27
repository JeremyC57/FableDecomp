// behaviour test for 0x0040F5E0: __uninitialized_copy copies [first, last) and returns result + (last - first)
#include <cstdio>
struct E { long a, b, c, d; };
static E* ucopy(const E* first, const E* last, E* result) { E* cur = result; for (; first != last; ++first, ++cur) *cur = *first; return cur; }
int main()
{
    E src[3] = { {1, 2, 3, 4}, {5, 6, 7, 8}, {9, 10, 11, 12} }; E dst[4];
    dst[3].a = -1;
    E* end = ucopy(src, src + 3, dst);
    int ok = end == dst + 3 && dst[2].d == 12 && dst[0].a == 1 && dst[3].a == -1 && ucopy(src, src, dst) == dst;
    if (!ok) { std::printf("0040f5e0_TEST FAIL\n"); return 1; }
    std::printf("0040f5e0_TEST PASS\n");
    return 0;
}