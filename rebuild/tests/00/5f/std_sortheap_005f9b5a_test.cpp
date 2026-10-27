// behaviour test for 0x005F9B5A std::sort_heap (model: a max-heap sorted ascending)
#include <cstdio>
#include <algorithm>
int main()
{
    int a[6] = { 9, 7, 8, 3, 5, 1 };
    std::make_heap(a, a + 6);
    int* last = a + 6;
    while (last - a > 1) std::pop_heap(a, last--);
    int ok = 1; for (int i = 1; i < 6; ++i) if (a[i - 1] > a[i]) ok = 0;
    if (!ok) { std::printf("005f9b5a_TEST FAIL\n"); return 1; }
    std::printf("005f9b5a_TEST PASS\n");
    return 0;
}