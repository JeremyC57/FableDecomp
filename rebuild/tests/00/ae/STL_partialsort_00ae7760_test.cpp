// behaviour test for 0x00AE7760 std::__partial_sort over pair<long, unsigned long>: the first middle-first elements become the smallest, sorted
#include <cstdio>
struct SPair { long first; unsigned int second; };
inline bool operator<(const SPair& x, const SPair& y) { return x.first < y.first || (!(y.first < x.first) && x.second < y.second); }
struct SLess { bool operator()(const SPair& x, const SPair& y) const { return x < y; } };
inline SPair P(long f, unsigned int s) { SPair p; p.first = f; p.second = s; return p; }
static bool Sorted(const SPair* a, int n) { for (int i = 1; i < n; ++i) if (a[i] < a[i - 1]) return false; return true; }
static long Sum(const SPair* a, int n) { long s = 0; for (int i = 0; i < n; ++i) s += a[i].first * 31 + (long)a[i].second; return s; }

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

    SPair a[10] = { P(9,0), P(3,1), P(7,0), P(1,0), P(8,0), P(3,0), P(6,0), P(0,2), P(5,0), P(2,0) };
    long before = Sum(a, 10);
    tst::partial_sort_(a, a + 4, a + 10, SLess(), (SPair*)0);
    if (!Sorted(a, 4) || Sum(a, 10) != before) { std::printf("00ae7760_TEST FAIL\n"); return 1; }
    if (a[0].first != 0 || a[1].first != 1 || a[2].first != 2 || a[3].first != 3 || a[3].second != 0) { std::printf("00ae7760_TEST FAIL\n"); return 1; }
    for (int i = 4; i < 10; ++i) if (a[i] < a[3]) { std::printf("00ae7760_TEST FAIL\n"); return 1; }
    std::printf("00ae7760_TEST PASS\n");
    return 0;
}