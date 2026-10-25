// Behaviour test for 0x006BB040 __introsort_loop over (unsigned long key, counted pointer) pairs (model copy).
#include <cstdio>

struct Entry { unsigned long first; long data; long info; };
struct Less { bool operator()(const Entry& a, const Entry& b) const { return a.first < b.first; } };

static int g_partial = 0;
static const Entry& median(const Entry& a, const Entry& b, const Entry& c, Less comp)
{
    if (comp(a, b)) { if (comp(b, c)) return b; else if (comp(a, c)) return c; else return a; }
    else if (comp(a, c)) return a;
    else if (comp(b, c)) return c;
    else return b;
}
static Entry* partition(Entry* first, Entry* last, Entry pivot, Less comp)
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
static void partial_sort_all(Entry* first, Entry* last, Less comp)      // stands in for the heap-sort fallback
{
    ++g_partial;
    for (Entry* i = first; i != last; ++i)
        for (Entry* j = i + 1; j != last; ++j)
            if (comp(*j, *i)) { Entry t = *i; *i = *j; *j = t; }
}
static void introsort_loop(Entry* first, Entry* last, int depth_limit, Less comp)
{
    while (last - first > 16) {
        if (depth_limit == 0) { partial_sort_all(first, last, comp); return; }
        --depth_limit;
        Entry* cut = partition(first, last, Entry(median(*first, *(first + (last - first) / 2), *(last - 1), comp)), comp);
        introsort_loop(cut, last, depth_limit, comp);
        last = cut;
    }
}
static void final_insertion(Entry* first, Entry* last, Less comp)
{
    for (Entry* i = first + 1; i < last; ++i) {
        Entry v = *i; Entry* j = i;
        while (j != first && comp(v, *(j - 1))) { *j = *(j - 1); --j; }
        *j = v;
    }
}

int main()
{
    Entry e[100];
    unsigned long seed = 12345;
    for (int i = 0; i < 100; ++i) { seed = seed * 1103515245UL + 12345UL; e[i].first = (seed >> 8) % 1000; e[i].data = i; e[i].info = 0; }
    Less comp;
    introsort_loop(e, e + 100, 14, comp);
    // after the loop every run of <= 16 elements is unsorted only internally: insertion sort finishes it
    final_insertion(e, e + 100, comp);
    int ok = 1;
    for (int i = 1; i < 100; ++i) if (e[i - 1].first > e[i].first) ok = 0;
    Entry f[40];
    for (int i = 0; i < 40; ++i) { f[i].first = 40 - i; f[i].data = i; f[i].info = 0; }
    introsort_loop(f, f + 40, 0, comp);                                // depth 0: the fallback sorts everything
    for (int i = 1; i < 40; ++i) if (f[i - 1].first > f[i].first) ok = 0;
    if (g_partial != 1) ok = 0;
    if (!ok) { std::printf("006bb040_TEST FAIL\n"); return 1; }
    std::printf("006bb040_TEST PASS\n");
    return 0;
}