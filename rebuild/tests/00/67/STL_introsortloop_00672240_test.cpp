// Behaviour test for 0x00672240: STLport __introsort_loop (median-of-3 partition down to 16-element runs, heap fallback)
#include <cstdio>

static int g_partialCalls = 0;
static void PartialSortAll(int* first, int* last)     // stands in for __partial_sort(first, last, last)
{
    ++g_partialCalls;
    for (int* i = first + 1; i < last; ++i) { int v = *i; int* j = i; while (j > first && v < j[-1]) { *j = j[-1]; --j; } *j = v; }
}
static const int& Median(const int& a, const int& b, const int& c)
{
    if (a < b) { if (b < c) return b; else if (a < c) return c; else return a; }
    else if (a < c) return a;
    else if (b < c) return c;
    else return b;
}
static int* Partition(int* first, int* last, int pivot)
{
    for (;;) {
        while (*first < pivot) ++first;
        --last;
        while (pivot < *last) --last;
        if (!(first < last)) return first;
        int t = *first; *first = *last; *last = t;
        ++first;
    }
}
static void IntroLoop(int* first, int* last, int depth)
{
    while (last - first > 16) {
        if (depth == 0) { PartialSortAll(first, last); return; }
        --depth;
        int* cut = Partition(first, last, int(Median(*first, *(first + (last - first) / 2), *(last - 1))));
        IntroLoop(cut, last, depth);
        last = cut;
    }
}

int main()
{
    int a[200];
    unsigned int seed = 12345;
    for (int i = 0; i < 200; ++i) { seed = seed * 1103515245u + 12345u; a[i] = (int)((seed >> 16) % 1000) - 500; }
    IntroLoop(a, a + 200, 14);
    // after the loop every block of the partition is <=16 long and ordered relative to the next: check with a final insertion sort count
    int moves = 0;
    for (int i = 1; i < 200; ++i) { int v = a[i]; int j = i; while (j > 0 && v < a[j - 1]) { a[j] = a[j - 1]; --j; ++moves; if (i - j > 16) { std::printf("00672240_TEST FAIL\n"); return 1; } } a[j] = v; }
    for (int i = 1; i < 200; ++i) if (a[i - 1] > a[i]) { std::printf("00672240_TEST FAIL\n"); return 1; }
    int b[40];
    for (int i = 0; i < 40; ++i) b[i] = 40 - i;
    IntroLoop(b, b + 40, 0);
    if (g_partialCalls != 1) { std::printf("00672240_TEST FAIL\n"); return 1; }
    for (int i = 0; i < 40; ++i) if (b[i] != i + 1) { std::printf("00672240_TEST FAIL\n"); return 1; }
    std::printf("00672240_TEST PASS\n");
    return 0;
}