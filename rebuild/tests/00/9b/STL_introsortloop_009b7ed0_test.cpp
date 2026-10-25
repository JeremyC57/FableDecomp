#include <cstdio>

// Host model: the CVectorMap element pair<unsigned int, int> (8 bytes in retail) under CKeyPairCompareLess.
template <class T1, class T2> struct pair {
    T1 first;
    T2 second;
};
typedef pair<unsigned int, int> TEntry;
struct CKeyPairCompareLess {
    bool operator()(const TEntry& a, const TEntry& b) const { return a.first < b.first; }
};
static TEntry g_v[80];
static void fill(int n, unsigned seed)
{
    for (int i = 0; i < n; ++i) {
        seed = seed * 1103515245u + 12345u;
        g_v[i].first = (unsigned int)((seed >> 9) % 200) - (unsigned int)0;
        g_v[i].second = (int)i;
    }
}
static bool permutation_ok(int n)                       // every payload still present exactly once
{
    for (int i = 0; i < n; ++i) { int seen = 0; for (int j = 0; j < n; ++j) if ((int)g_v[j].second == i) ++seen; if (seen != 1) return false; }
    return true;
}

// ---- copy of the STLport 4.x sort/heap algorithms the retail TU instantiates (plain templates for the host) ----
namespace model {
template <class T> inline void iter_swap_(T* a, T* b) { T t = *a; *a = *b; *b = t; }
template <class T, class C> inline const T& median_(const T& a, const T& b, const T& c, C comp)
{
    if (comp(a, b))
        if (comp(b, c)) return b; else if (comp(a, c)) return c; else return a;
    else if (comp(a, c)) return a;
    else if (comp(b, c)) return c;
    else return b;
}
template <class T, class C> T* unguarded_partition(T* first, T* last, T pivot, C comp)
{
    for (;;) {
        while (comp(*first, pivot)) ++first;
        --last;
        while (comp(pivot, *last)) --last;
        if (!(first < last)) return first;
        iter_swap_(first, last);
        ++first;
    }
}
template <class T, class C> void unguarded_linear_insert(T* last, T val, C comp)
{
    T* next = last; --next;
    while (comp(val, *next)) { *last = *next; last = next; --next; }
    *last = val;
}
template <class T, class C> void linear_insert(T* first, T* last, T val, C comp)
{
    if (comp(val, *first)) {
        for (T* p = last; p != first; --p) *p = *(p - 1);
        *first = val;
    } else
        unguarded_linear_insert(last, val, comp);
}
template <class T, class C> void insertion_sort(T* first, T* last, C comp)
{
    if (first == last) return;
    for (T* i = first + 1; i != last; ++i) linear_insert(first, i, *i, comp);
}
template <class T, class C> void unguarded_insertion_sort(T* first, T* last, C comp)
{
    for (T* i = first; i != last; ++i) unguarded_linear_insert(i, *i, comp);
}
template <class T, class C> void final_insertion_sort(T* first, T* last, C comp)
{
    if (last - first > 16) {
        insertion_sort(first, first + 16, comp);
        unguarded_insertion_sort(first + 16, last, comp);
    } else
        insertion_sort(first, last, comp);
}
template <class T, class C> void push_heap_(T* first, int hole, int top, T val, C comp)
{
    int parent = (hole - 1) / 2;
    while (hole > top && comp(*(first + parent), val)) { *(first + hole) = *(first + parent); hole = parent; parent = (hole - 1) / 2; }
    *(first + hole) = val;
}
template <class T, class C> void adjust_heap(T* first, int hole, int len, T val, C comp)
{
    int top = hole;
    int second = 2 * hole + 2;
    while (second < len) {
        if (comp(*(first + second), *(first + (second - 1)))) second--;
        *(first + hole) = *(first + second);
        hole = second;
        second = 2 * (second + 1);
    }
    if (second == len) { *(first + hole) = *(first + (second - 1)); hole = second - 1; }
    push_heap_(first, hole, top, val, comp);
}
template <class T, class C> void make_heap_(T* first, T* last, C comp)
{
    if (last - first < 2) return;
    int len = (int)(last - first);
    int parent = (len - 2) / 2;
    for (;;) {
        adjust_heap(first, parent, len, T(*(first + parent)), comp);
        if (parent == 0) return;
        parent--;
    }
}
template <class T, class C> void sort_heap_(T* first, T* last, C comp)
{
    while (last - first > 1) {
        --last;
        T val = *last;
        *last = *first;
        adjust_heap(first, 0, (int)(last - first), val, comp);
    }
}
template <class T, class C> void partial_sort_(T* first, T* middle, T* last, C comp)
{
    make_heap_(first, middle, comp);
    for (T* i = middle; i < last; ++i)
        if (comp(*i, *first)) { T val = *i; *i = *first; adjust_heap(first, 0, (int)(middle - first), val, comp); }
    sort_heap_(first, middle, comp);
}
inline int lg_(int n) { int k; for (k = 0; n != 1; n >>= 1) ++k; return k; }
template <class T, class C> void introsort_loop(T* first, T* last, int depth, C comp)
{
    while (last - first > 16) {
        if (depth == 0) { partial_sort_(first, last, last, comp); return; }
        --depth;
        T* cut = unguarded_partition(first, last, T(median_(*first, *(first + (last - first) / 2), *(last - 1), comp)), comp);
        introsort_loop(cut, last, depth, comp);
        last = cut;
    }
}
template <class T, class C> void sort_(T* first, T* last, C comp)
{
    if (first != last) {
        introsort_loop(first, last, lg_((int)(last - first)) * 2, comp);
        final_insertion_sort(first, last, comp);
    }
}
template <class T, class C> bool is_sorted_(const T* first, const T* last, C comp)
{
    for (const T* p = first; p + 1 < last; ++p) if (comp(*(p + 1), *p)) return false;
    return true;
}
}
// ---- end of the algorithm copy ----

int main()
{
    CKeyPairCompareLess comp;
    fill(70, 21);
    g_v[3].first = 0xFFFFFFF0u;                               // unsigned key: a huge value sorts last
    model::introsort_loop(g_v, g_v + 70, 12, comp);
    model::final_insertion_sort(g_v, g_v + 70, comp);
    if (!model::is_sorted_(g_v, g_v + 70, comp)) { std::printf("009b7ed0_TEST FAIL\n"); return 1; }
    if (g_v[69].second != 3) { std::printf("009b7ed0_TEST FAIL\n"); return 1; }
    if (!permutation_ok(70)) { std::printf("009b7ed0_TEST FAIL\n"); return 1; }
    fill(70, 22);
    model::introsort_loop(g_v, g_v + 70, 0, comp);
    if (!model::is_sorted_(g_v, g_v + 70, comp)) { std::printf("009b7ed0_TEST FAIL\n"); return 1; }
    std::printf("009b7ed0_TEST PASS\n");
    return 0;
}