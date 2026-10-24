#include <cstdio>
// Behaviour test for 0x006FB9C0: _Push_heap over pair<unsigned long, CSmallVector<CEvent,32> > (deep-copying
// small vector; the by-value argument frees its buffer).
struct CEvent { long m_a, m_b; };
static int g_deletes = 0;
template<class T, int N> struct CSmallVector {
    T* Start;
    int Size;
    CSmallVector() : Start(0), Size(0) { }
    void Copy(const CSmallVector& other)
    {
        if (Start) { delete[] Start; ++g_deletes; }
        Start = 0; Size = 0;
        Start = new T[other.Size ? other.Size : 1];
        Size = other.Size;
        for (int i = 0; i < Size; ++i) Start[i] = other.Start[i];
    }
    CSmallVector(const CSmallVector& other) : Start(0), Size(0) { Copy(other); }
    CSmallVector& operator=(const CSmallVector& other) { if (&other != this) Copy(other); return *this; }
    ~CSmallVector() { if (Start) ++g_deletes; delete[] Start; }
};
template<class K, class V> struct pair { K first; V second; };
template<class K, class V> struct CKeyPairCompareLess {
    bool operator()(const pair<K, V>& a, const pair<K, V>& b) const { return a.first < b.first; }
    bool operator()(const pair<K, V>& entry, const K& key) const { return entry.first < key; }
};
template<class _RanIt, class _Diff, class _Ty, class _Pr>
inline void _Push_heap(_RanIt _First, _Diff _Hole, _Diff _Top, _Ty _Val, _Pr _Pred)
{
    const unsigned long _Key = _Val.first;
    for (_Diff _Idx = (_Hole - 1) / 2; _Hole > _Top && _Pred(*(_First + _Idx), _Key); _Idx = (_Hole - 1) / 2) {
        *(_First + _Hole) = *(_First + _Idx);
        _Hole = _Idx;
    }
    (_First + _Hole)->first = _Key;
    (_First + _Hole)->second = _Val.second;
}
typedef pair<unsigned long, CSmallVector<CEvent, 32> > Elem;
int main()
{
    int fail = 0;
    {
        Elem h[8]; int n = 0;
        unsigned long keys[7] = { 3, 0x80000000u, 1, 7, 0x80000001u, 2, 5 };
        for (int i = 0; i < 7; ++i) {
            Elem v; v.first = keys[i];
            v.second.Start = new CEvent[i + 1]; v.second.Size = i + 1;
            for (int e = 0; e <= i; ++e) { v.second.Start[e].m_a = i; v.second.Start[e].m_b = e; }
            int d0 = g_deletes;
            _Push_heap(h, n, 0, v, CKeyPairCompareLess<unsigned long, CSmallVector<CEvent, 32> >()); ++n;
            if (g_deletes <= d0) fail = 1;          // the by-value copy freed its buffer
            for (int c = 1; c < n; ++c)
                if (h[(c - 1) / 2].first < h[c].first) fail = 1;
        }
        if (h[0].first != 0x80000001u || h[0].second.Size != 5 || h[0].second.Start[4].m_a != 4 || h[0].second.Start[4].m_b != 4) fail = 1;
        for (int c = 0; c < n; ++c)
            if (h[c].second.Size < 1 || h[c].second.Start[0].m_a + 1 != h[c].second.Size) fail = 1;
    }
    if (fail) { std::printf("006fb9c0_TEST FAIL\n"); return 1; }
    std::printf("006fb9c0_TEST PASS\n");
    return 0;
}