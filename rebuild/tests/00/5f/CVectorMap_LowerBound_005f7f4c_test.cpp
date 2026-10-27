// behaviour test for 0x005F7F4C: LowerBound finds the first entry whose (signed) key is not below the probe
#include <cstdio>
struct E { long key; long v; };
static const E* LowerBound(const E* first, const E* last, long key)
{
    int count = (int)(last - first);
    while (count > 0) { int half = count / 2; const E* m = first + half; if (m->key < key) { first = m + 1; count -= half + 1; } else count = half; }
    return first;
}
int main()
{
    E a[4] = { { -5 }, { 0 }, { 3 }, { 9 } };
    if (LowerBound(a, a + 4, -6) != a || LowerBound(a, a + 4, 1) != a + 2 || LowerBound(a, a + 4, 10) != a + 4) { std::printf("005f7f4c_TEST FAIL\n"); return 1; }
    std::printf("005f7f4c_TEST PASS\n");
    return 0;
}