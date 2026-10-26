#include <cstdio>

// Host model: CWideString as a pointer to a zero-terminated UTF-16 buffer with the lexicographic operator<; the
// two-argument sort passes an (empty) less<> functor to every helper.
class CWideString { public:
    const unsigned short* Chars;
    bool operator<(const CWideString& other) const
    {
        const unsigned short* a = Chars;
        const unsigned short* b = other.Chars;
        for (;;) {
            if (*a != *b) return *a < *b;
            if (*a == 0) return false;
            ++a; ++b;
        }
    }
};
template <class T> struct less {
    bool operator()(const T& x, const T& y) const { return x < y; }
};
template <class T> void sort_two_arg(T* first, T* last);
static unsigned short g_text[64][6];
static CWideString g_s[64];

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

template <class T> void sort_two_arg(T* first, T* last)
{
    if (first != last) {
        model::introsort_loop(first, last, model::lg_((int)(last - first)) * 2, less<T>());
        model::final_insertion_sort(first, last, less<T>());
    }
}

int main()
{
    unsigned seed = 77;
    for (int i = 0; i < 64; ++i) {
        for (int j = 0; j < 5; ++j) { seed = seed * 1103515245u + 12345u; g_text[i][j] = (unsigned short)('a' + (seed >> 12) % 4); }
        g_text[i][5] = 0;
        g_s[i].Chars = g_text[i];
    }
    sort_two_arg(g_s, g_s + 64);
    less<CWideString> comp;
    if (!model::is_sorted_(g_s, g_s + 64, comp)) { std::printf("009b8440_TEST FAIL\n"); return 1; }
    for (int i = 0; i < 64; ++i) { int seen = 0; for (int j = 0; j < 64; ++j) if (g_s[j].Chars == g_text[i]) ++seen; if (seen != 1) { std::printf("009b8440_TEST FAIL\n"); return 1; } }
    sort_two_arg(g_s, g_s);                                   // empty range: nothing happens
    std::printf("009b8440_TEST PASS\n");
    return 0;
}