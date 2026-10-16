#include <cstdio>
// Behaviour model of __linear_insert (0x009B58B0): one insertion-sort step over a sorted prefix [first, last).
static void UnguardedLinearInsert(int* last, int value)
{
    int* next = last - 1;
    while (value < *next) { *last = *next; last = next; --next; }
    *last = value;
}
static void LinearInsert(int* first, int* last, int value)
{
    if (value < *first) {
        for (int* p = last; p != first; --p) *p = *(p - 1);
        *first = value;
    }
    else
        UnguardedLinearInsert(last, value);
}
int main()
{
    int a[5] = { 2, 4, 6, 0, 0 };
    LinearInsert(a, a + 3, 1);
    LinearInsert(a, a + 4, 5);
    if (a[0] != 1 || a[1] != 2 || a[2] != 4 || a[3] != 5 || a[4] != 6) { std::printf("009b58b0_TEST FAIL\n"); return 1; }
    std::printf("009b58b0_TEST PASS\n");
    return 0;
}
