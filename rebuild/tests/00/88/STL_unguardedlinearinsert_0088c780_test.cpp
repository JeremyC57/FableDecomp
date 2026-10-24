#include <cstdio>
#ifndef _MSC_VER
#define __fastcall
#endif
// Model of _STL::__unguarded_linear_insert<SKeyedPair*, SKeyedPair, SKeyGreater> @ 0x0088C780: shift the pairs whose key
// is smaller than val's up one place (descending order) and drop val into the gap.
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
void __fastcall UnguardedLinearInsert(_RandomAccessIter __last, _Tp __val, _Compare __comp)
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
int main()
{
    bool ok = true;
    SKeyedPair v[5] = { SKeyedPair(9, 0), SKeyedPair(7, 1), SKeyedPair(5, 2), SKeyedPair(3, 3), SKeyedPair(6, 4) };
    UnguardedLinearInsert(v + 4, v[4], SKeyGreater());
    int keys[5] = { 9, 7, 6, 5, 3 };
    unsigned int vals[5] = { 0, 1, 4, 2, 3 };
    for (int i = 0; i < 5; ++i)
        if (v[i].m_key != keys[i] || v[i].m_value != vals[i]) ok = false;
    SKeyedPair w[3] = { SKeyedPair(4, 0), SKeyedPair(2, 1), SKeyedPair(2, 2) };   // equal key: stays after its twin
    UnguardedLinearInsert(w + 2, w[2], SKeyGreater());
    if (w[1].m_value != 1 || w[2].m_value != 2) ok = false;
    if (!ok) { std::printf("0088c780_TEST FAIL\n"); return 1; }
    std::printf("0088c780_TEST PASS\n");
    return 0;
}