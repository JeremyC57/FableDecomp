// Behaviour test for STLport __introsort_loop over the decal map entries (0x00BEFA20): ((long, long) key, payload)
// entries are partitioned around the median-of-three pivot down to runs of at most 16, with the heap-sort fallback when
// the depth budget is spent; afterwards every run is key-bounded so a final insertion sort sorts the whole range.
#include <cstdio>

struct Key { long a, b; };
struct Entry { Key first; long payload; };
static bool keyLess(const Key& x, const Key& y) { return x.a < y.a || (!(y.a < x.a) && x.b < y.b); }
struct Less { bool operator()(const Entry& x, const Entry& y) const { return keyLess(x.first, y.first); } };

static int g_partialSorts = 0;

static const Entry& median(const Entry& a, const Entry& b, const Entry& c, Less comp)
{
    if (comp(a, b)) { if (comp(b, c)) return b; else if (comp(a, c)) return c; else return a; }
    else if (comp(a, c)) return a;
    else if (comp(b, c)) return c;
    else return b;
}
static Entry* unguarded_partition(Entry* first, Entry* last, Entry pivot, Less comp)
{
    for (;;) {
        while (comp(*first, pivot)) ++first;
        --last;
        while (comp(pivot, *last)) --last;
        if (!(first < last)) return first;
        Entry t = *first; *first = *last; *last = t;
        ++first;
    }
}
static void partial_sort_all(Entry* first, Entry* last, Less comp)
{
    ++g_partialSorts;
    for (Entry* i = first + 1; i < last; ++i)
        for (Entry* j = i; j > first && comp(*j, *(j - 1)); --j) { Entry t = *j; *j = *(j - 1); *(j - 1) = t; }
}
static void introsort_loop(Entry* first, Entry* last, int depth_limit, Less comp)
{
    while (last - first > 16) {
        if (depth_limit == 0) { partial_sort_all(first, last, comp); return; }
        --depth_limit;
        Entry* cut = unguarded_partition(first, last, Entry(median(*first, *(first + (last - first) / 2), *(last - 1), comp)), comp);
        introsort_loop(cut, last, depth_limit, comp);
        last = cut;
    }
}
static bool sorted(const Entry* f, const Entry* l) { for (const Entry* i = f + 1; i < l; ++i) if (keyLess(i->first, (i - 1)->first)) return false; return true; }

int main()
{
    Entry e[200];
    unsigned int seed = 12345u;
    int i;
    for (i = 0; i < 200; ++i) { seed = seed * 1103515245u + 12345u; e[i].first.a = (long)((seed >> 16) % 7); e[i].first.b = (long)((seed >> 8) % 50); e[i].payload = i; }
    introsort_loop(e, e + 200, 20, Less());
    bool ok = g_partialSorts == 0;
    // every element of a later run is not less than any element of an earlier run: an insertion sort finishes it
    partial_sort_all(e, e + 200, Less());
    ok = ok && sorted(e, e + 200);
    long sum = 0; for (i = 0; i < 200; ++i) sum += e[i].payload;
    ok = ok && sum == 199L * 200L / 2L;
    // depth 0 on a long range goes straight to the heap-sort fallback
    for (i = 0; i < 40; ++i) { e[i].first.a = 40 - i; e[i].first.b = 0; e[i].payload = i; }
    g_partialSorts = 0;
    introsort_loop(e, e + 40, 0, Less());
    ok = ok && g_partialSorts == 1 && sorted(e, e + 40) && e[0].first.a == 1;
    // a short range (<= 16) is left alone
    for (i = 0; i < 16; ++i) { e[i].first.a = 16 - i; e[i].first.b = 0; }
    introsort_loop(e, e + 16, 5, Less());
    ok = ok && e[0].first.a == 16 && e[15].first.a == 1;
    std::printf(ok ? "00befa20_TEST PASS\n" : "00befa20_TEST FAIL\n");
    return 0;
}