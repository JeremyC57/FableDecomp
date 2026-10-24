// Behaviour test for 0x00594E4D std::__introsort_loop (model copy; the leftover runs are finished by an insertion sort)
#include <cstdio>
struct Entry { int id; int priority; };
struct PriorityCompare { bool operator()(const Entry& l, const Entry& r) const { return l.priority > r.priority; } };
static int g_partial_sorts = 0;
static void partial_sort_all(Entry* first, Entry* last, PriorityCompare comp)
{
    ++g_partial_sorts;
    for (Entry* i = first + 1; i < last; ++i) { Entry v = *i; Entry* j = i; while (j != first && comp(v, *(j - 1))) { *j = *(j - 1); --j; } *j = v; }
}
static const Entry& median(const Entry& a, const Entry& b, const Entry& c, PriorityCompare comp)
{
    if (comp(a, b)) { if (comp(b, c)) return b; else if (comp(a, c)) return c; else return a; }
    else if (comp(a, c)) return a; else if (comp(b, c)) return c; else return b;
}
static Entry* unguarded_partition(Entry* first, Entry* last, Entry pivot, PriorityCompare comp)
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
static void introsort_loop(Entry* first, Entry* last, Entry*, int depth_limit, PriorityCompare comp)
{
    while (last - first > 16) {
        if (depth_limit == 0) { partial_sort_all(first, last, comp); return; }
        --depth_limit;
        Entry* cut = unguarded_partition(first, last, Entry(median(*first, *(first + (last - first) / 2), *(last - 1), comp)), comp);
        introsort_loop(cut, last, (Entry*)0, depth_limit, comp);
        last = cut;
    }
}
int main()
{
    Entry v[100];
    unsigned int seed = 12345;
    for (int i = 0; i < 100; ++i) { seed = seed * 1103515245u + 12345u; v[i].id = i; v[i].priority = int((seed >> 16) % 1000); }
    introsort_loop(v, v + 100, (Entry*)0, 14, PriorityCompare());
    partial_sort_all(v, v + 100, PriorityCompare());     // the driver's final insertion sort
    for (int i = 1; i < 100; ++i) if (v[i - 1].priority < v[i].priority) { std::printf("00594e4d_TEST FAIL\n"); return 1; }
    g_partial_sorts = 0;
    for (int i = 0; i < 40; ++i) v[i].priority = i;
    introsort_loop(v, v + 40, (Entry*)0, 0, PriorityCompare());   // depth 0: straight to the heap-sort fallback
    if (g_partial_sorts != 1 || v[0].priority != 39 || v[39].priority != 0) { std::printf("00594e4d_TEST FAIL\n"); return 1; }
    std::printf("00594e4d_TEST PASS\n");
    return 0;
}