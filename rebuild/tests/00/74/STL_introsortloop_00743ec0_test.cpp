// Behaviour test for 0x00743EC0 std::__introsort_loop over the mode-registry pairs (unsigned key)
#include <cstdio>
struct CRegisteredModeInfo { int tag; };

template <class A, class B> struct pair {
    A first;
    B second;
    pair() {}
    pair(const pair& other) : first(other.first), second(other.second) {}
};

typedef pair<unsigned int, CRegisteredModeInfo> TModePair;   // unsigned int: the 32-bit unsigned key

struct TModeLess {
    bool operator()(const TModePair& a, const TModePair& b) const { return a.first < b.first; }
};

static void Fill(TModePair* p, const unsigned int* keys, int n)
{
    for (int i = 0; i < n; ++i) { p[i].first = keys[i]; p[i].second.tag = i; }
}

template <class T, class Compare>
inline const T& __median(const T& a, const T& b, const T& c, Compare comp)
{
    if (comp(a, b))
        if (comp(b, c))
            return b;
        else if (comp(a, c))
            return c;
        else
            return a;
    else if (comp(a, c))
        return a;
    else if (comp(b, c))
        return c;
    else
        return b;
}

// stand-ins for the out-of-line callees
TModePair* __unguarded_partition(TModePair* first, TModePair* last, TModePair pivot, TModeLess comp)
{
    for (;;) {
        while (comp(*first, pivot))
            ++first;
        --last;
        while (comp(pivot, *last))
            --last;
        if (!(first < last))
            return first;
        TModePair tmp(*first);
        *first = *last;
        *last = tmp;
        ++first;
    }
}
static int g_partialSorts = 0;
void __partial_sort(TModePair* first, TModePair* middle, TModePair* last, TModePair*, TModeLess comp)
{
    ++g_partialSorts;                                   // middle == last here: a plain full sort will do
    for (TModePair* i = first + 1; i < last; ++i)
        for (TModePair* j = i; j > first && comp(*j, *(j - 1)); --j) {
            TModePair tmp(*j);
            *j = *(j - 1);
            *(j - 1) = tmp;
        }
}
inline void partial_sort(TModePair* first, TModePair* middle, TModePair* last, TModeLess comp)
{
    __partial_sort(first, middle, last, (TModePair*)0, comp);
}

void __introsort_loop(TModePair* first, TModePair* last, TModePair*, int depth_limit, TModeLess comp)
{
    while (last - first > 16) {
        if (depth_limit == 0) {
            partial_sort(first, last, last, comp);
            return;
        }
        --depth_limit;
        TModePair* cut = __unguarded_partition(first, last,
                                               TModePair(__median(*first, *(first + (last - first) / 2), *(last - 1), comp)),
                                               comp);
        __introsort_loop(cut, last, (TModePair*)0, depth_limit, comp);
        last = cut;
    }
}

static unsigned int Key(int i) { return (unsigned int)(i * 2654435761u) ^ (i & 1 ? 0x80000000u : 0u); }

int main()
{
    const int N = 100;
    TModePair p[N];
    for (int i = 0; i < N; ++i) { p[i].first = Key(i); p[i].second.tag = i; }
    __introsort_loop(p, p + N, (TModePair*)0, 2 * 6, TModeLess());
    int ok = 1;
    // after the loop the range is a sequence of blocks no longer than 16, each block's keys all
    // above the previous block's: a single bounded insertion pass must finish it
    unsigned int sumBefore = 0, sumAfter = 0;
    for (int i = 0; i < N; ++i) sumBefore += Key(i);
    for (int i = 0; i < N; ++i) sumAfter += p[i].first;
    if (sumBefore != sumAfter) ok = 0;
    for (int i = 1; i < N; ++i) {
        int back = 0;
        for (int j = i; j > 0 && p[j].first < p[j - 1].first; --j) {
            TModePair tmp(p[j]); p[j] = p[j - 1]; p[j - 1] = tmp;
            if (++back > 16) ok = 0;
        }
    }
    for (int i = 1; i < N; ++i) if (p[i - 1].first > p[i].first) ok = 0;
    for (int i = 0; i < N; ++i) if (Key(p[i].second.tag) != p[i].first) ok = 0;   // values travel with their keys
    if (g_partialSorts != 0) ok = 0;
    // depth limit 0 on a long range: the partial-sort fallback sorts it outright
    for (int i = 0; i < 40; ++i) { p[i].first = Key(i); p[i].second.tag = i; }
    __introsort_loop(p, p + 40, (TModePair*)0, 0, TModeLess());
    if (g_partialSorts != 1) ok = 0;
    for (int i = 1; i < 40; ++i) if (p[i - 1].first > p[i].first) ok = 0;
    // a run of 16 or fewer is left alone
    for (int i = 0; i < 16; ++i) { p[i].first = 16u - i; p[i].second.tag = i; }
    __introsort_loop(p, p + 16, (TModePair*)0, 8, TModeLess());
    for (int i = 0; i < 16; ++i) if (p[i].first != 16u - i) ok = 0;
    if (!ok) { std::printf("00743ec0_TEST FAIL\n"); return 1; }
    std::printf("00743ec0_TEST PASS\n");
    return 0;
}