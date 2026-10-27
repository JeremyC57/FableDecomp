// behaviour test for 0x004D1AE0: the final insertion sort orders short and long ranges
#include <cstdio>
static void InsertionSort(int* f, int* l) { if (f == l) return; for (int* i = f + 1; i != l; ++i) { int v = *i; int* j = i; while (j != f && v < *(j - 1)) { *j = *(j - 1); --j; } *j = v; } }
static void Unguarded(int* f, int* l) { for (int* i = f; i != l; ++i) { int v = *i; int* j = i; while (v < *(j - 1)) { *j = *(j - 1); --j; } *j = v; } }
static void Final(int* f, int* l) { if (l - f > 16) { InsertionSort(f, f + 16); Unguarded(f + 16, l); } else InsertionSort(f, l); }
int main()
{
    int a[20]; for (int i = 0; i < 20; ++i) a[i] = (i * 7) % 20;
    int b[5] = { 3, 1, 4, 1, 5 };
    Final(a, a + 20); Final(b, b + 5);
    int ok = b[0] == 1 && b[4] == 5;
    for (int i = 1; i < 20; ++i) ok = ok && a[i - 1] <= a[i];
    if (!ok) { std::printf("004d1ae0_TEST FAIL\n"); return 1; }
    std::printf("004d1ae0_TEST PASS\n");
    return 0;
}