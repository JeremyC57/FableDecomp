// Behaviour test for 0x00AE7BE0 std::__introsort_loop over pair<long, unsigned long> with less<pair>
#include <cstdio>

struct P { int first; unsigned int second; };
static bool lessp(const P& x, const P& y) { return x.first < y.first || (!(y.first < x.first) && x.second < y.second); }
static int g_partialCalls = 0;

static const P& median3(const P& a, const P& b, const P& c)
{
    if (lessp(a, b))
        if (lessp(b, c)) return b; else if (lessp(a, c)) return c; else return a;
    else if (lessp(a, c)) return a;
    else if (lessp(b, c)) return c;
    else return b;
}

static P* partition(P* first, P* last, P pivot)
{
    for (;;) {
        while (lessp(*first, pivot)) ++first;
        --last;
        while (lessp(pivot, *last)) --last;
        if (!(first < last)) return first;
        P t = *first; *first = *last; *last = t;
        ++first;
    }
}

static void partial_sort_all(P* first, P* last)
{
    ++g_partialCalls;
    for (P* i = first + 1; i < last; ++i)
        for (P* j = i; j > first && lessp(*j, *(j - 1)); --j) { P t = *j; *j = *(j - 1); *(j - 1) = t; }
}

static void introsort_loop(P* first, P* last, int depth)
{
    while (last - first > 16) {
        if (depth == 0) { partial_sort_all(first, last); return; }
        --depth;
        P* cut = partition(first, last, median3(*first, *(first + (last - first) / 2), *(last - 1)));
        introsort_loop(cut, last, depth);
        last = cut;
    }
}

int main()
{
    P a[150];
    unsigned int seed = 777;
    for (int i = 0; i < 150; ++i) {
        seed = seed * 1103515245u + 12345u; a[i].first = (int)((seed >> 8) % 7) - 3;
        seed = seed * 1103515245u + 12345u; a[i].second = (seed >> 4);
    }
    introsort_loop(a, a + 150, 20);
    for (int i = 0; i < 150; ++i)
        for (int j = i + 17; j < 150; ++j)
            if (lessp(a[j], a[i])) { std::printf("00ae7be0_TEST FAIL\n"); return 1; }
    if (g_partialCalls != 0) { std::printf("00ae7be0_TEST FAIL\n"); return 1; }
    introsort_loop(a, a + 150, 0);
    for (int i = 1; i < 150; ++i) if (lessp(a[i], a[i - 1])) { std::printf("00ae7be0_TEST FAIL\n"); return 1; }
    if (g_partialCalls != 1) { std::printf("00ae7be0_TEST FAIL\n"); return 1; }
    std::printf("00ae7be0_TEST PASS\n");
    return 0;
}