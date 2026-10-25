// behaviour test for 0x00B867A0 std::__introsort_loop over CSortElement (SortValue order)
#include <cstdio>

struct SortElement { unsigned int SortValue; int Tag; };
static bool Less(const SortElement& a, const SortElement& b) { return a.SortValue < b.SortValue; }

static const SortElement& Median(const SortElement& a, const SortElement& b, const SortElement& c)
{
    if (Less(a, b)) { if (Less(b, c)) return b; else if (Less(a, c)) return c; else return a; }
    else if (Less(a, c)) return a;
    else if (Less(b, c)) return c;
    else return b;
}

static SortElement* UnguardedPartition(SortElement* first, SortElement* last, SortElement pivot)
{
    for (;;) {
        while (Less(*first, pivot)) ++first;
        --last;
        while (Less(pivot, *last)) --last;
        if (!(first < last)) return first;
        SortElement t = *first; *first = *last; *last = t;
        ++first;
    }
}

static void SiftDown(SortElement* a, int i, int n)
{
    for (;;) {
        int c = 2 * i + 1;
        if (c >= n) return;
        if (c + 1 < n && Less(a[c], a[c + 1])) ++c;
        if (!Less(a[i], a[c])) return;
        SortElement t = a[i]; a[i] = a[c]; a[c] = t;
        i = c;
    }
}

static int PartialSorts = 0;
static void PartialSortAll(SortElement* first, SortElement* last)   // partial_sort(first, last, last): a heap sort
{
    ++PartialSorts;
    int n = (int)(last - first);
    for (int i = n / 2 - 1; i >= 0; --i) SiftDown(first, i, n);
    for (int k = n - 1; k > 0; --k) { SortElement t = first[0]; first[0] = first[k]; first[k] = t; SiftDown(first, 0, k); }
}

static void IntrosortLoop(SortElement* first, SortElement* last, int depth)
{
    while (last - first > 16) {
        if (depth == 0) { PartialSortAll(first, last); return; }
        --depth;
        SortElement* cut = UnguardedPartition(first, last, SortElement(Median(*first, *(first + (last - first) / 2), *(last - 1))));
        IntrosortLoop(cut, last, depth);
        last = cut;
    }
}

static void InsertionSort(SortElement* first, SortElement* last)
{
    for (SortElement* i = first + 1; i < last; ++i) {
        SortElement v = *i; SortElement* j = i;
        while (j > first && Less(v, *(j - 1))) { *j = *(j - 1); --j; }
        *j = v;
    }
}

int main()
{
    SortElement a[200];
    unsigned int seed = 12345;
    for (int i = 0; i < 200; ++i) { seed = seed * 1103515245u + 12345u; a[i].SortValue = seed; a[i].Tag = i; }
    IntrosortLoop(a, a + 200, 2 * 7);
    // every 16-or-fewer block left is bounded by its neighbours: a final insertion sort finishes it
    InsertionSort(a, a + 200);
    for (int i = 1; i < 200; ++i)
        if (a[i - 1].SortValue > a[i].SortValue) { std::printf("00b867a0_TEST FAIL\n"); return 1; }
    SortElement b[100];
    for (int i = 0; i < 100; ++i) { b[i].SortValue = (unsigned int)(100 - i); b[i].Tag = i; }
    IntrosortLoop(b, b + 100, 0);   // depth exhausted: heap sort the whole range
    if (PartialSorts != 1) { std::printf("00b867a0_TEST FAIL\n"); return 1; }
    for (int i = 0; i < 100; ++i)
        if (b[i].SortValue != (unsigned int)(i + 1)) { std::printf("00b867a0_TEST FAIL\n"); return 1; }
    std::printf("00b867a0_TEST PASS\n");
    return 0;
}