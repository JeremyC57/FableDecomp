// Behaviour test for 0x0086DB30: introsort loop (median-of-3 partition, recurse right, stop at 16, heap fallback at depth 0)
#include <cstdio>
#include <cstdlib>

struct Entry { long key; long a; long b; long c; };

static bool Less(const Entry& x, const Entry& y) { return x.key < y.key; }

static const Entry& Median(const Entry& a, const Entry& b, const Entry& c)
{
    if (Less(a, b)) { if (Less(b, c)) return b; else if (Less(a, c)) return c; else return a; }
    else if (Less(a, c)) return a;
    else if (Less(b, c)) return c;
    else return b;
}

static Entry* Partition(Entry* first, Entry* last, Entry pivot)
{
    for (;;) {
        while (Less(*first, pivot)) ++first;
        --last;
        while (Less(pivot, *last)) --last;
        if (!(first < last)) return first;
        Entry t = *first; *first = *last; *last = t;
        ++first;
    }
}

static int g_fallbacks = 0;

static void HeapFallbackSort(Entry* first, Entry* last)        // stands in for partial_sort(first, last, last)
{
    ++g_fallbacks;
    for (Entry* i = first + 1; i < last; ++i)
        for (Entry* j = i; j > first && Less(*j, *(j - 1)); --j) { Entry t = *j; *j = *(j - 1); *(j - 1) = t; }
}

static void IntrosortLoop(Entry* first, Entry* last, int depth)
{
    while (last - first > 16) {
        if (depth == 0) { HeapFallbackSort(first, last); return; }
        --depth;
        Entry* cut = Partition(first, last, Entry(Median(*first, *(first + (last - first) / 2), *(last - 1))));
        IntrosortLoop(cut, last, depth);
        last = cut;
    }
}

static void FinalInsertion(Entry* first, Entry* last)
{
    for (Entry* i = first + 1; i < last; ++i)
        for (Entry* j = i; j > first && Less(*j, *(j - 1)); --j) { Entry t = *j; *j = *(j - 1); *(j - 1) = t; }
}

int main()
{
    Entry e[200];
    unsigned int r = 99u;
    for (int i = 0; i < 200; ++i) { r = r * 1103515245u + 12345u; e[i].key = (long)((r >> 8) % 1000u) - 500; e[i].a = e[i].key * 2; e[i].b = i; e[i].c = 1; }
    IntrosortLoop(e, e + 200, 16);
    // after the loop every 16-element block boundary is partitioned: a final insertion sort finishes it
    FinalInsertion(e, e + 200);
    for (int i = 1; i < 200; ++i)
        if (e[i - 1].key > e[i].key || e[i].a != e[i].key * 2) { std::printf("0086db30_TEST FAIL\n"); return 1; }
    if (g_fallbacks != 0) { std::printf("0086db30_TEST FAIL\n"); return 1; }
    Entry f[40];
    for (int i = 0; i < 40; ++i) { f[i].key = 40 - i; f[i].a = f[i].key * 2; }
    IntrosortLoop(f, f + 40, 0);                                // depth 0: the fallback sorts the whole range
    if (g_fallbacks != 1) { std::printf("0086db30_TEST FAIL\n"); return 1; }
    for (int i = 0; i < 40; ++i) if (f[i].key != i + 1) { std::printf("0086db30_TEST FAIL\n"); return 1; }
    std::printf("0086db30_TEST PASS\n");
    return 0;
}