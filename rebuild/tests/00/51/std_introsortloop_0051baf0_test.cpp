#include <cstdio>
// Behaviour model of STLport __introsort_loop with out-of-line median / partition / partial_sort over 4-byte elements.
typedef int T;
struct Less { bool operator()(T a, T b) const { return a < b; } };
static int g_partial;
static const T& median(const T& a, const T& b, const T& c, Less comp)
{
    if (comp(a, b)) { if (comp(b, c)) return b; else if (comp(a, c)) return c; else return a; }
    else if (comp(a, c)) return a; else if (comp(b, c)) return c; else return b;
}
static T* partition(T* first, T* last, T pivot, Less comp)
{
    for (;;) {
        while (comp(*first, pivot)) ++first;
        --last;
        while (comp(pivot, *last)) --last;
        if (!(first < last)) return first;
        T t = *first; *first = *last; *last = t;
        ++first;
    }
}
static void partial_sort_(T* first, T* middle, T* last, Less comp)
{
    ++g_partial;
    for (T* i = first; i != middle; ++i)
        for (T* j = i + 1; j != last; ++j)
            if (comp(*j, *i)) { T t = *i; *i = *j; *j = t; }
}
static void introsort_loop(T* first, T* last, T*, int depth_limit, Less comp)
{
    while (last - first > 16) {
        if (depth_limit == 0) { partial_sort_(first, last, last, comp); return; }
        --depth_limit;
        T* cut = partition(first, last, T(median(*first, *(first + (last - first) / 2), *(last - 1), comp)), comp);
        introsort_loop(cut, last, (T*)0, depth_limit, comp);
        last = cut;
    }
}
static void insertion_sort(T* first, T* last)
{
    for (T* i = first + 1; i < last; ++i) { T v = *i; T* j = i; while (j != first && v < *(j - 1)) { *j = *(j - 1); --j; } *j = v; }
}
int main()
{
    T a[100];
    unsigned int seed = 12345;
    for (int i = 0; i < 100; ++i) { seed = seed * 1103515245u + 12345u; a[i] = (int)((seed >> 8) % 1000); }
    introsort_loop(a, a + 100, (T*)0, 12, Less());
    insertion_sort(a, a + 100);
    for (int i = 1; i < 100; ++i) if (a[i - 1] > a[i]) { std::printf("0051baf0_TEST FAIL\n"); return 1; }
    if (g_partial != 0) { std::printf("0051baf0_TEST FAIL\n"); return 1; }
    T b[40];
    for (int i = 0; i < 40; ++i) b[i] = 40 - i;
    introsort_loop(b, b + 40, (T*)0, 0, Less());          // depth exhausted: partial_sort the whole range
    for (int i = 0; i < 40; ++i) if (b[i] != i + 1) { std::printf("0051baf0_TEST FAIL\n"); return 1; }
    if (g_partial != 1) { std::printf("0051baf0_TEST FAIL\n"); return 1; }
    T c[10] = { 5, 4, 3, 2, 1, 0, 9, 8, 7, 6 };
    introsort_loop(c, c + 10, (T*)0, 3, Less());          // short range: untouched
    if (c[0] != 5 || c[9] != 6) { std::printf("0051baf0_TEST FAIL\n"); return 1; }
    std::printf("0051baf0_TEST PASS\n");
    return 0;
}