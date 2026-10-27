// behaviour test for 0x006B8AE0: LowerBound finds the first entry whose key is not below the probe
#include <cstdio>
struct E { unsigned long key; unsigned long v[2]; };
static const E* LowerBound(const E* first, const E* last, unsigned long key)
{
    int count = (int)(last - first);
    while (count > 0) { int half = count / 2; const E* m = first + half; if (m->key < key) { first = m + 1; count -= half + 1; } else count = half; }
    return first;
}
int main()
{
    E a[4] = { { 2 }, { 5 }, { 9 }, { 0x80000000UL } };
    if (LowerBound(a, a + 4, 5) != a + 1 || LowerBound(a, a + 4, 6) != a + 2 || LowerBound(a, a + 4, 0x90000000UL) != a + 4 || LowerBound(a, a, 1) != a) { std::printf("006b8ae0_TEST FAIL\n"); return 1; }
    std::printf("006b8ae0_TEST PASS\n");
    return 0;
}