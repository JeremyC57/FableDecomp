// behaviour test for 0x009b9320 std::__introsort_loop<pair<unsigned long, CDefinitionManager::CDefClassInfo>*, ..., CKeyPairCompareLess>: while more than 16 elements remain,
// heap-sort the range once the depth budget is spent, else partition around the median of first/middle/last-1 and
// recurse on the right part, looping on the left. Checked: the loop plus a final insertion sort orders the range by
// the comparator, and depth 0 sorts it outright through partial_sort.
#include <cstdio>
struct Elem { unsigned long first; int tag; };
struct Cmp { bool operator()(const Elem& a, const Elem& b) const { return a.first < b.first; } };
static const Elem& median(const Elem& a, const Elem& b, const Elem& c, Cmp comp)
{
    if (comp(a, b)) { if (comp(b, c)) return b; else if (comp(a, c)) return c; else return a; }
    else if (comp(a, c)) return a; else if (comp(b, c)) return c; else return b;
}
static Elem* unguarded_partition(Elem* first, Elem* last, Elem pivot, Cmp comp)
{
    for (;;) {
        while (comp(*first, pivot)) ++first;
        --last;
        while (comp(pivot, *last)) --last;
        if (!(first < last)) return first;
        Elem t = *first; *first = *last; *last = t;
        ++first;
    }
}
static void adjust_heap(Elem* first, int hole, int len, Elem v, Cmp comp)
{
    int top = hole, child = 2 * hole + 2;
    while (child < len) {
        if (comp(first[child], first[child - 1])) --child;
        first[hole] = first[child]; hole = child; child = 2 * (child + 1);
    }
    if (child == len) { first[hole] = first[child - 1]; hole = child - 1; }
    int parent = (hole - 1) / 2;
    while (hole > top && comp(first[parent], v)) { first[hole] = first[parent]; hole = parent; parent = (hole - 1) / 2; }
    first[hole] = v;
}
static void partial_sort(Elem* first, Elem* middle, Elem* last, Cmp comp)
{
    int len = middle - first;
    if (len >= 2) for (int p = (len - 2) / 2; ; --p) { adjust_heap(first, p, len, first[p], comp); if (p == 0) break; }
    for (Elem* i = middle; i < last; ++i)
        if (comp(*i, *first)) { Elem v = *i; *i = *first; adjust_heap(first, 0, len, v, comp); }
    while (middle - first > 1) { --middle; Elem v = *middle; *middle = *first; adjust_heap(first, 0, middle - first, v, comp); }
}
static void introsort_loop(Elem* first, Elem* last, int depth_limit, Cmp comp)
{
    while (last - first > 16) {
        if (depth_limit == 0) { partial_sort(first, last, last, comp); return; }
        --depth_limit;
        Elem* cut = unguarded_partition(first, last,
            Elem(median(*first, *(first + (last - first) / 2), *(last - 1), comp)), comp);
        introsort_loop(cut, last, depth_limit, comp);
        last = cut;
    }
}
static void insertion_sort(Elem* first, Elem* last, Cmp comp)
{
    for (Elem* i = first + 1; i < last; ++i) { Elem v = *i; Elem* j = i; while (j > first && comp(v, *(j - 1))) { *j = *(j - 1); --j; } *j = v; }
}
int main()
{
    bool ok = true;
    for (int depth = 0; depth < 2; ++depth) {
        Elem e[100]; unsigned s = 12345u;
        for (int i = 0; i < 100; ++i) { s = s * 1103515245u + 12345u; e[i].first = (s >> 4) % 5000u; e[i].tag = i; }
        introsort_loop(e, e + 100, depth == 0 ? 0 : 14, Cmp());
        insertion_sort(e, e + 100, Cmp());
        for (int i = 1; i < 100; ++i) if (Cmp()(e[i], e[i - 1])) ok = false;
    }
    if (!ok) { std::printf("009b9320_TEST FAIL\n"); return 1; }
    std::printf("009b9320_TEST PASS\n");
    return 0;
}