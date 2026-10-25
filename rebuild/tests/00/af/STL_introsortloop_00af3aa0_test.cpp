// behaviour test for 0x00AF3AA0 std::__introsort_loop over pair<CCharString, CRegisteredPSCInfo> (key compare): partition down to runs of 16, heap sort once the depth runs out
#include <cstdio>
#include <cstring>
struct CStr { const char* s; };
inline bool operator<(const CStr& a, const CStr& b) { return std::strcmp(a.s, b.s) < 0; }
struct SPSCPair { CStr first; int info; };
struct SKeyLess { bool operator()(const SPSCPair& a, const SPSCPair& b) const { return a.first < b.first; } };
static const char* gNames[] = { "Wind", "Colour", "Size", "Spin", "Alpha", "Gravity", "Spline", "Emit", "Decal", "Light",
    "Mesh", "Trail", "Noise", "Attract", "Bounce", "Fade", "Orbit", "Pulse", "Ribbon", "Scale" };
inline SPSCPair Q(int i) { SPSCPair p; p.first.s = gNames[i]; p.info = i; return p; }
static bool Sorted(const SPSCPair* a, int n) { for (int i = 1; i < n; ++i) if (a[i].first < a[i - 1].first) return false; return true; }
static int Sum(const SPSCPair* a, int n) { int s = 0; for (int i = 0; i < n; ++i) s += a[i].info; return s; }

namespace tst {
template <class It, class T, class C> void unguarded_linear_insert(It last, T val, C comp)
{
    It next = last;
    --next;
    while (comp(val, *next)) { *last = *next; last = next; --next; }
    *last = val;
}
template <class It, class T, class C> void linear_insert(It first, It last, T val, C comp)
{
    if (comp(val, *first)) {
        for (It p = last; p != first; --p) *p = *(p - 1);
        *first = val;
    }
    else
        unguarded_linear_insert(last, val, comp);
}
template <class It, class T, class C> It unguarded_partition(It first, It last, T pivot, C comp)
{
    for (;;) {
        while (comp(*first, pivot)) ++first;
        --last;
        while (comp(pivot, *last)) --last;
        if (!(first < last)) return first;
        T tmp = *first; *first = *last; *last = tmp;
        ++first;
    }
}
template <class It, class C> void unguarded_insertion_sort(It first, It last, C comp)
{
    for (It i = first; i != last; ++i) unguarded_linear_insert(i, *i, comp);
}
template <class It, class T, class C> void push_heap_(It first, int hole, int top, T value, C comp)
{
    int parent = (hole - 1) / 2;
    while (hole > top && comp(first[parent], value)) { first[hole] = first[parent]; hole = parent; parent = (hole - 1) / 2; }
    first[hole] = value;
}
template <class It, class T, class C> void adjust_heap(It first, int hole, int len, T value, C comp)
{
    int top = hole;
    int second = 2 * hole + 2;
    while (second < len) {
        if (comp(first[second], first[second - 1])) second--;
        first[hole] = first[second];
        hole = second;
        second = 2 * (second + 1);
    }
    if (second == len) { first[hole] = first[second - 1]; hole = second - 1; }
    push_heap_(first, hole, top, value, comp);
}
template <class It, class T, class C> void make_heap_(It first, It last, C comp, T*)
{
    if (last - first < 2) return;
    int len = (int)(last - first);
    int parent = (len - 2) / 2;
    for (;;) {
        T v = first[parent];
        adjust_heap(first, parent, len, v, comp);
        if (parent == 0) return;
        parent--;
    }
}
template <class It, class T, class C> void pop_heap_(It first, It last, It result, T value, C comp)
{
    *result = *first;
    adjust_heap(first, 0, (int)(last - first), value, comp);
}
template <class It, class T, class C> void sort_heap_(It first, It last, C comp, T*)
{
    while (last - first > 1) { --last; T v = *last; pop_heap_(first, last, last, v, comp); }
}
template <class It, class T, class C> void partial_sort_(It first, It middle, It last, C comp, T* t)
{
    make_heap_(first, middle, comp, t);
    for (It i = middle; i < last; ++i)
        if (comp(*i, *first)) { T v = *i; pop_heap_(first, middle, i, v, comp); }
    sort_heap_(first, middle, comp, t);
}
template <class T, class C> const T& median(const T& a, const T& b, const T& c, C comp)
{
    if (comp(a, b)) {
        if (comp(b, c)) return b;
        else if (comp(a, c)) return c;
        else return a;
    }
    else if (comp(a, c)) return a;
    else if (comp(b, c)) return c;
    else return b;
}
template <class It, class T, class C> void introsort_loop(It first, It last, T* t, int depth, C comp)
{
    while (last - first > 16) {
        if (depth == 0) { partial_sort_(first, last, last, comp, t); return; }
        --depth;
        T pivot = median(*first, *(first + (last - first) / 2), *(last - 1), comp);
        It cut = unguarded_partition(first, last, pivot, comp);
        introsort_loop(cut, last, t, depth, comp);
        last = cut;
    }
}
}
int main()
{

    SPSCPair a[20];
    for (int i = 0; i < 20; ++i) a[i] = Q(i);
    int before = Sum(a, 20);
    tst::introsort_loop(a, a + 20, (SPSCPair*)0, 0, SKeyLess());   // depth 0: partial_sort of the whole range
    if (!Sorted(a, 20) || Sum(a, 20) != before) { std::printf("00af3aa0_TEST FAIL\n"); return 1; }
    SPSCPair b[40];
    for (int i = 0; i < 40; ++i) b[i] = Q((i * 7) % 20);
    int beforeb = Sum(b, 40);
    tst::introsort_loop(b, b + 40, (SPSCPair*)0, 8, SKeyLess());
    for (int i = 1; i < 40; ++i) tst::linear_insert(b, b + i, SPSCPair(b[i]), SKeyLess());   // the final insertion sort
    if (!Sorted(b, 40) || Sum(b, 40) != beforeb) { std::printf("00af3aa0_TEST FAIL\n"); return 1; }
    std::printf("00af3aa0_TEST PASS\n");
    return 0;
}