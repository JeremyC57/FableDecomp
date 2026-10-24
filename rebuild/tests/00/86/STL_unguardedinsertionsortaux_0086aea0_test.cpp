// Behaviour test for 0x0086AEA0: __unguarded_insertion_sort_aux sorts [first, last) by the unsigned key, given a sentinel
// entry before first that is not greater than any key (the "unguarded" precondition); values travel with their keys.
#include <cstdio>

struct SValue { unsigned int Words[2]; };
struct SPair
{
    unsigned int first;
    SValue second;
};

struct SCompare { bool operator()(const SPair& a, const SPair& b) const { return a.first < b.first; } };

template <class _RandomAccessIter, class _Tp, class _Compare>
void unguarded_linear_insert(_RandomAccessIter __last, _Tp __val, _Compare __comp)
{
    _RandomAccessIter __next = __last;
    --__next;
    while (__comp(__val, *__next)) { *__last = *__next; __last = __next; --__next; }
    *__last = __val;
}

template <class _RandomAccessIter, class _Tp, class _Compare>
void unguarded_insertion_sort_aux(_RandomAccessIter __first, _RandomAccessIter __last, _Tp*, _Compare __comp)
{
    for (_RandomAccessIter __i = __first; __i != __last; ++__i)
        unguarded_linear_insert(__i, _Tp(*__i), __comp);
}

int main()
{
    SPair a[7];
    unsigned int keys[7] = { 0, 9, 4, 7, 4, 1, 8 };
    for (int i = 0; i < 7; ++i) { a[i].first = keys[i]; a[i].second.Words[0] = 100 + i; a[i].second.Words[1] = (unsigned int)keys[i] * 3u; }
    int fails = 0;
    if (sizeof(SPair) != 12u) ++fails;
    unguarded_insertion_sort_aux(a + 1, a + 7, (SPair*)0, SCompare());
    for (int i = 1; i < 7; ++i) if (a[i].first < a[i - 1].first) ++fails;
    for (int i = 0; i < 7; ++i) if (a[i].second.Words[1] != (unsigned int)a[i].first * 3u) ++fails;
    if (a[1].first != 1 || a[6].first != 9) ++fails;
    if (a[2].second.Words[0] != 102 || a[3].second.Words[0] != 104) ++fails;    // equal keys keep their order
    if (fails) { std::printf("0086aea0_TEST FAIL\n"); return 1; }
    std::printf("0086aea0_TEST PASS\n");
    return 0;
}