#include <cstdio>
struct SEntry { long key; long tag; };
struct CLess { bool operator()(const SEntry& a, const SEntry& b) const { return a.key < b.key; } };
static int g_partialSorts = 0, g_partitions = 0;
static void PartialSort(SEntry* first, SEntry* middle, SEntry* last, CLess comp)
{
    ++g_partialSorts;                                       // selection sort stand-in with the same contract
    for (SEntry* i = first; i != middle; ++i) {
        SEntry* m = i;
        for (SEntry* j = i + 1; j != last; ++j) if (comp(*j, *m)) m = j;
        SEntry t = *i; *i = *m; *m = t;
    }
}
static const SEntry& Median(const SEntry& a, const SEntry& b, const SEntry& c, CLess comp)
{
    if (comp(a, b)) { if (comp(b, c)) return b; else if (comp(a, c)) return c; else return a; }
    else if (comp(a, c)) return a;
    else if (comp(b, c)) return c;
    else return b;
}
static SEntry* UnguardedPartition(SEntry* first, SEntry* last, SEntry pivot, CLess comp)
{
    ++g_partitions;
    for (;;) {
        while (comp(*first, pivot)) ++first;
        --last;
        while (comp(pivot, *last)) --last;
        if (!(first < last)) return first;
        SEntry t = *first; *first = *last; *last = t;
        ++first;
    }
}
static void IntrosortLoop(SEntry* first, SEntry* last, SEntry*, int depth_limit, CLess comp)
{
    while (last - first > 16) {
        if (depth_limit == 0) { PartialSort(first, last, last, comp); return; }
        --depth_limit;
        SEntry* cut = UnguardedPartition(first, last, SEntry(Median(*first, *(first + (last - first) / 2), *(last - 1), comp)), comp);
        IntrosortLoop(cut, last, (SEntry*)0, depth_limit, comp);
        last = cut;
    }
}
static void InsertionSort(SEntry* first, SEntry* last, CLess comp)
{
    for (SEntry* i = first + 1; i < last; ++i) { SEntry v = *i; SEntry* j = i; while (j != first && comp(v, *(j - 1))) { *j = *(j - 1); --j; } *j = v; }
}
static bool Sorted(const SEntry* a, int n) { for (int i = 1; i < n; ++i) if (a[i].key < a[i - 1].key) return false; return true; }
int main()
{
    bool ok = true;
    SEntry a[100];
    for (int i = 0; i < 100; ++i) { a[i].key = (i * 37 + 11) % 100; a[i].tag = i; }
    IntrosortLoop(a, a + 100, 0, 14, CLess());
    if (g_partitions == 0 || g_partialSorts != 0) ok = false;
    InsertionSort(a, a + 100, CLess());
    if (!Sorted(a, 100)) ok = false;
    SEntry b[40];
    for (int i = 0; i < 40; ++i) { b[i].key = 40 - i; b[i].tag = i; }
    g_partitions = 0;
    IntrosortLoop(b, b + 40, 0, 0, CLess());                // no depth budget: one partial_sort of the whole range
    if (g_partialSorts != 1 || g_partitions != 0 || !Sorted(b, 40)) ok = false;
    SEntry c[10];
    for (int i = 0; i < 10; ++i) { c[i].key = 10 - i; c[i].tag = i; }
    IntrosortLoop(c, c + 10, 0, 5, CLess());                // 16 or fewer: untouched
    if (c[0].key != 10 || c[9].key != 1) ok = false;
    if (!ok) { std::printf("004e823b_TEST FAIL\n"); return 1; }
    std::printf("004e823b_TEST PASS\n");
    return 0;
}