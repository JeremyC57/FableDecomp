// Behaviour test for std::__introsort_loop<pair<CCharString, CCountedPointer<...> >*, ..., CCompareString<...> > 0x00CBDAD0:
// the STLport introsort loop partitions runs longer than 16 elements around the median-of-three pivot (recursing on the right
// part, looping on the left) and falls back to partial_sort when the depth limit is exhausted; combined with the final insertion
// sort of sort() every element ends in key order with its payload intact.
#include <cstdio>
#include <cstring>
#include <algorithm>

class CCharString { public: const char* text; bool operator<(const CCharString& o) const { return strcmp(text, o.text) < 0; } };
struct CCountedPointer { int* data; long* refs; };
struct Pair { CCharString first; CCountedPointer second; };
struct CCompareString { bool operator()(const Pair& a, const Pair& b) const { return a.first < b.first; } };

template <class T, class C> const T& median3(const T& a, const T& b, const T& c, C comp)
{
    if (comp(a, b)) { if (comp(b, c)) return b; else if (comp(a, c)) return c; else return a; }
    else if (comp(a, c)) return a; else if (comp(b, c)) return c; else return b;
}
template <class T, class C> T* unguarded_partition(T* first, T* last, T pivot, C comp)
{
    for (;;) {
        while (comp(*first, pivot)) ++first;
        --last;
        while (comp(pivot, *last)) --last;
        if (!(first < last)) return first;
        std::swap(*first, *last);
        ++first;
    }
}
template <class T, class C> void partial_sort_all(T* first, T* last, C comp) { std::sort(first, last, comp); }   // partial_sort(first, last, last)
template <class T, class C> void introsort_loop(T* first, T* last, int depth_limit, C comp)
{
    while (last - first > 16) {
        if (depth_limit == 0) { partial_sort_all(first, last, comp); return; }
        --depth_limit;
        T* cut = unguarded_partition(first, last, T(median3(*first, *(first + (last - first) / 2), *(last - 1), comp)), comp);
        introsort_loop(cut, last, depth_limit, comp);
        last = cut;
    }
}
template <class T, class C> void insertion_sort(T* first, T* last, C comp)
{
    for (T* i = first + 1; i < last; ++i) { T v = *i; T* j = i; while (j > first && comp(v, *(j - 1))) { *j = *(j - 1); --j; } *j = v; }
}

int main()
{
    const int n = 40;
    static const char* keys[n] = { "zeta","alpha","mike","bravo","yankee","kilo","echo","xray","golf","hotel","india","juliet","lima","november","oscar","papa","quebec","romeo","sierra","tango","uniform","victor","whiskey","charlie","delta","foxtrot","aa","zz","mm","bb","yy","kk","ee","xx","gg","hh","ii","jj","ll","nn" };
    Pair a[n]; int payload[n]; long refs[n];
    for (int i = 0; i < n; ++i) { a[i].first.text = keys[i]; payload[i] = i; refs[i] = 1; a[i].second.data = &payload[i]; a[i].second.refs = &refs[i]; }
    CCompareString comp;
    int fails = 0;
    introsort_loop(a, a + n, 2 * 5, comp);     // 2 * lg(40)
    insertion_sort(a, a + n, comp);
    for (int i = 1; i < n; ++i) if (strcmp(a[i - 1].first.text, a[i].first.text) > 0) ++fails;
    for (int i = 0; i < n; ++i) if (strcmp(keys[*a[i].second.data], a[i].first.text) != 0) ++fails;   // payload travelled with its key
    // depth limit 0: the loop hands the whole run to partial_sort
    Pair b[n]; for (int i = 0; i < n; ++i) b[i] = a[n - 1 - i];
    introsort_loop(b, b + n, 0, comp);
    for (int i = 1; i < n; ++i) if (strcmp(b[i - 1].first.text, b[i].first.text) > 0) ++fails;
    // a short run (<= 16) is left to the insertion sort
    Pair c[5]; for (int i = 0; i < 5; ++i) c[i] = a[4 - i];
    introsort_loop(c, c + 5, 4, comp);
    if (strcmp(c[0].first.text, a[4].first.text) != 0) ++fails;
    if (fails == 0) printf("00cbdad0_TEST PASS\n"); else printf("00cbdad0_TEST FAIL %d\n", fails);
    return fails != 0;
}