// behaviour test for std::__introsort_loop<CActiveScriptPair*, ...> (0x00CBD700): the STLport 4.6 loop over (key, value) pairs
// ordered by key; after the loop every run between partition points is < 16 elements and the final insertion sort sorts all
#include <cstdio>

#ifdef _MSC_VER
typedef unsigned __int64 Key;
#else
typedef unsigned long long Key;
#endif
struct Pair { Key first; int second; };
struct Less { bool operator()(const Pair& a, const Pair& b) const { return a.first < b.first; } };

static const int __stl_threshold = 16;
static int g_partialSorts = 0;

static const Pair& __median(const Pair& a, const Pair& b, const Pair& c, Less comp)
{
    if (comp(a, b))
        if (comp(b, c)) return b;
        else if (comp(a, c)) return c;
        else return a;
    else if (comp(a, c)) return a;
    else if (comp(b, c)) return c;
    else return b;
}
static Pair* __unguarded_partition(Pair* first, Pair* last, Pair pivot, Less comp)
{
    for (;;) {
        while (comp(*first, pivot)) ++first;
        --last;
        while (comp(pivot, *last)) --last;
        if (!(first < last)) return first;
        Pair t = *first; *first = *last; *last = t;
        ++first;
    }
}
static void __partial_sort(Pair* first, Pair* middle, Pair* last, Less comp)   // the heap path (stand-in: selection)
{
    ++g_partialSorts;
    for (Pair* i = first; i < middle; ++i) {
        Pair* m = i;
        for (Pair* j = i + 1; j < last; ++j) if (comp(*j, *m)) m = j;
        Pair t = *i; *i = *m; *m = t;
    }
}
static void __introsort_loop(Pair* first, Pair* last, Pair*, int depth_limit, Less comp)
{
    while (last - first > __stl_threshold) {
        if (depth_limit == 0) {
            __partial_sort(first, last, last, comp);
            return;
        }
        --depth_limit;
        Pair* cut = __unguarded_partition(first, last, Pair(__median(*first, *(first + (last - first) / 2), *(last - 1), comp)), comp);
        __introsort_loop(cut, last, (Pair*)0, depth_limit, comp);
        last = cut;
    }
}
static void insertion_sort(Pair* first, Pair* last)
{
    for (Pair* i = first + 1; i < last; ++i) {
        Pair v = *i; Pair* j = i;
        while (j > first && v.first < (j - 1)->first) { *j = *(j - 1); --j; }
        *j = v;
    }
}
static int lg(int n) { int k = 0; for (; n != 1; n >>= 1) ++k; return k; }

int main()
{
    Pair a[300];
    unsigned int seed = 12345;
    for (int i = 0; i < 300; ++i) {
        seed = seed * 1103515245u + 12345u;
        a[i].first = ((Key)(seed >> 8) << 32) | (seed & 0xffff);   // keys need the full 64-bit compare
        a[i].second = i;
    }
    Less comp;
    __introsort_loop(a, a + 300, (Pair*)0, lg(300) * 2, comp);
    insertion_sort(a, a + 300);
    bool ok = true;
    for (int i = 1; i < 300; ++i) if (a[i].first < a[i - 1].first) ok = false;
    Pair b[64];
    for (int i = 0; i < 64; ++i) { b[i].first = (Key)(64 - i) << 33; b[i].second = i; }
    __introsort_loop(b, b + 64, (Pair*)0, 0, comp);                    // depth exhausted: the partial-sort fallback
    ok = ok && g_partialSorts == 1 && b[0].first == ((Key)1 << 33) && b[63].first == ((Key)64 << 33);
    if (!ok) { std::printf("00cbd700_TEST FAIL\n"); return 1; }
    std::printf("00cbd700_TEST PASS\n");
    return 0;
}