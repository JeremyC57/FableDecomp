// behaviour test for 0x00A34780: the final insertion sort orders short and long ranges alike
#include <cstdio>
#include <algorithm>
static void InsertionSort(long* f, long* l) { for (long* i = f + 1; i < l; ++i) { long v = *i; long* j = i; while (j > f && v < j[-1]) { *j = j[-1]; --j; } *j = v; } }
static void FinalInsertionSort(long* f, long* l) { if (l - f > 16) { InsertionSort(f, f + 16); InsertionSort(f, l); } else InsertionSort(f, l); }
int main()
{
    long a[40]; for (int i = 0; i < 40; ++i) a[i] = (i * 17) % 41;
    long b[5] = { 5, 1, 4, 2, 3 };
    FinalInsertionSort(a, a + 40); FinalInsertionSort(b, b + 5);
    for (int i = 1; i < 40; ++i) if (a[i - 1] > a[i]) { std::printf("00a34780_TEST FAIL\n"); return 1; }
    if (b[0] != 1 || b[4] != 5) { std::printf("00a34780_TEST FAIL\n"); return 1; }
    std::printf("00a34780_TEST PASS\n");
    return 0;
}