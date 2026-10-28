#include <cstdio>
#include <cstdlib>

// Test model of std::__introsort_loop over the symbol-map entries: median-of-three pivot, unguarded partition,
// right-part recursion, partial_sort (heap) fallback when the depth limit runs out. The key stands in for the
// CCharString compared by FastCompare; after the loop each 16-element block is unordered only inside itself,
// so the final insertion sort (the caller's job) completes the order.
struct SEntry { int Key; int GroupName; int EntryName; int Val; };
struct CCompare { bool operator()(const SEntry& a, const SEntry& b) const { return a.Key < b.Key; } };

static void Swap(SEntry* a, SEntry* b) { SEntry t = *a; *a = *b; *b = t; }

static const SEntry& Median(const SEntry& a, const SEntry& b, const SEntry& c, CCompare comp)
{
    if (comp(a, b))
        if (comp(b, c))
            return b;
        else if (comp(a, c))
            return c;
        else
            return a;
    else if (comp(a, c))
        return a;
    else if (comp(b, c))
        return c;
    else
        return b;
}

static SEntry* UnguardedPartition(SEntry* first, SEntry* last, SEntry pivot, CCompare comp)
{
    for (;;) {
        while (comp(*first, pivot))
            ++first;
        --last;
        while (comp(pivot, *last))
            --last;
        if (!(first < last))
            return first;
        Swap(first, last);
        ++first;
    }
}

static void PartialSort(SEntry* first, SEntry* middle, SEntry* last, CCompare comp)
{
    // simple selection-based stand-in for __partial_sort(first, last, last): sorts the whole range
    for (SEntry* i = first; i < last; ++i)
        for (SEntry* j = i + 1; j < last; ++j)
            if (comp(*j, *i))
                Swap(i, j);
    (void)middle;
}

static int g_partialSorts = 0;

static void IntrosortLoop(SEntry* first, SEntry* last, long depth_limit, CCompare comp)
{
    while (last - first > 16) {
        if (depth_limit == 0) {
            ++g_partialSorts;
            PartialSort(first, last, last, comp);
            return;
        }
        --depth_limit;
        SEntry* cut = UnguardedPartition(first, last,
                                         SEntry(Median(*first, *(first + (last - first) / 2), *(last - 1), comp)), comp);
        IntrosortLoop(cut, last, depth_limit, comp);
        last = cut;
    }
}

static void InsertionSort(SEntry* first, SEntry* last, CCompare comp)
{
    for (SEntry* i = first + 1; i < last; ++i)
        for (SEntry* j = i; j > first && comp(*j, *(j - 1)); --j)
            Swap(j, j - 1);
}

int main()
{
    const int n = 300;
    SEntry a[n];
    unsigned int seed = 12345u;
    for (int i = 0; i < n; ++i) {
        seed = seed * 1103515245u + 12345u;
        a[i].Key = (int)((seed >> 8) % 1000u);
        a[i].GroupName = a[i].EntryName = a[i].Val = a[i].Key;
    }
    CCompare comp;
    IntrosortLoop(a, a + n, 2 * 8, comp);
    InsertionSort(a, a + n, comp);
    int ok = 1;
    for (int i = 1; i < n; ++i)
        if (a[i - 1].Key > a[i].Key || a[i].Val != a[i].Key)
            ok = 0;
    // depth 0 goes straight to the partial-sort fallback
    SEntry b[40];
    for (int i = 0; i < 40; ++i) { b[i].Key = 40 - i; b[i].Val = b[i].Key; }
    g_partialSorts = 0;
    IntrosortLoop(b, b + 40, 0, comp);
    if (g_partialSorts != 1) ok = 0;
    for (int i = 1; i < 40; ++i)
        if (b[i - 1].Key > b[i].Key) ok = 0;
    // a short range (<= 16) is left alone
    SEntry c[10];
    for (int i = 0; i < 10; ++i) c[i].Key = 10 - i;
    IntrosortLoop(c, c + 10, 4, comp);
    if (c[0].Key != 10) ok = 0;
    if (!ok) { std::printf("009e7d30_TEST FAIL\n"); return 1; }
    std::printf("009e7d30_TEST PASS\n");
    return 0;
}