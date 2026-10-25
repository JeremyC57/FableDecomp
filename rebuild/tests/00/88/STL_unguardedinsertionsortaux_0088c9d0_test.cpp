#include <cstdio>
#ifndef _MSC_VER
#define __fastcall
#endif
// Model of std::__unguarded_insertion_sort_aux<SKeyedPair*, SKeyedPair, SKeyGreater> @ 0x0088C9D0: insertion-sort
// [first, last) descending by key, relying on a larger-or-equal key before first as the sentinel (introsort tail pass).
struct SKeyedPair {
    int          m_key;
    unsigned int m_value;
    SKeyedPair(int key, unsigned int value) : m_key(key), m_value(value) { }
    SKeyedPair(const SKeyedPair& other) : m_key(other.m_key), m_value(other.m_value) { }
};
struct SKeyGreater {
    bool operator()(const SKeyedPair& a, const SKeyedPair& b) const { return a.m_key > b.m_key; }
};
template <class _RandomAccessIter, class _Tp, class _Compare>
inline void __fastcall UnguardedLinearInsert(_RandomAccessIter __last, _Tp __val, _Compare __comp)
{
    _RandomAccessIter __next = __last;
    --__next;
    while (__comp(__val, *__next)) {
        *__last = *__next;
        __last = __next;
        --__next;
    }
    *__last = __val;
}
template <class _RandomAccessIter, class _Tp, class _Compare>
void __fastcall UnguardedInsertionSortAux(_RandomAccessIter __first, _RandomAccessIter __last, _Tp*, _Compare __comp)
{
    for (_RandomAccessIter __i = __first; __i != __last; ++__i)
        UnguardedLinearInsert(__i, _Tp(*__i), __comp);
}
int main()
{
    bool ok = true;
    SKeyedPair v[8] = { SKeyedPair(100, 0), SKeyedPair(50, 1), SKeyedPair(10, 2),     // sorted prefix (sentinel 100)
                        SKeyedPair(70, 3), SKeyedPair(10, 4), SKeyedPair(90, 5), SKeyedPair(-5, 6), SKeyedPair(50, 7) };
    UnguardedInsertionSortAux(v + 3, v + 8, (SKeyedPair*)0, SKeyGreater());
    int keys[8] = { 100, 90, 70, 50, 50, 10, 10, -5 };
    unsigned int vals[8] = { 0, 5, 3, 1, 7, 2, 4, 6 };                    // equal keys keep their order
    for (int i = 0; i < 8; ++i)
        if (v[i].m_key != keys[i] || v[i].m_value != vals[i]) ok = false;
    if (!ok) { std::printf("0088c9d0_TEST FAIL\n"); return 1; }
    std::printf("0088c9d0_TEST PASS\n");
    return 0;
}