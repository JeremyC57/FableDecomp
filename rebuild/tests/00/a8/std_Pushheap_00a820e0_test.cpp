#include <cstdio>
// Behaviour test for 0x00A820E0: _Push_heap over pair<unsigned __int64, 8-byte list> (out-of-line list
// assignment and destructor; the by-value list is destroyed).
#ifdef _MSC_VER
typedef unsigned __int64 u64;
#else
typedef unsigned long long u64;
#endif
static int g_assigns = 0, g_dtors = 0;
struct SList8 {
    void* m_pHead;
    long m_size;
    SList8() : m_pHead(0), m_size(0) { }
    SList8(const SList8& o) : m_pHead(o.m_pHead), m_size(o.m_size) { }
    SList8& operator=(const SList8& o) { m_pHead = o.m_pHead; m_size = o.m_size; ++g_assigns; return *this; }
    ~SList8() { ++g_dtors; }
};
template<class K, class V> struct pair { K first; V second; };
template<class K, class V> struct CKeyPairCompareLess {
    bool operator()(const pair<K, V>& a, const pair<K, V>& b) const { return a.first < b.first; }
};
template<class _RanIt, class _Diff, class _Ty, class _Pr>
inline void _Push_heap(_RanIt _First, _Diff _Hole, _Diff _Top, _Ty _Val, _Pr _Pred)
{
    for (_Diff _Idx = (_Hole - 1) / 2; _Hole > _Top && _Pred(*(_First + _Idx), _Val); _Idx = (_Hole - 1) / 2) {
        *(_First + _Hole) = *(_First + _Idx);
        _Hole = _Idx;
    }
    *(_First + _Hole) = _Val;
}
typedef pair<u64, SList8> Elem;
int main()
{
    Elem h[10]; int n = 0;
    u64 keys[8] = { 9, ((u64)3 << 32), 0xFFFFFFFFu, ((u64)3 << 32) + 7, 1, 0, ((u64)1 << 33), 5 };
    for (int i = 0; i < 8; ++i) {
        Elem v; v.first = keys[i]; v.second.m_size = i;
        int d0 = g_dtors;
        _Push_heap(h, n, 0, v, CKeyPairCompareLess<u64, SList8>()); ++n;
        if (g_dtors <= d0) { std::printf("00a820e0_TEST FAIL\n"); return 1; }
        for (int c = 1; c < n; ++c)
            if (h[(c - 1) / 2].first < h[c].first) { std::printf("00a820e0_TEST FAIL\n"); return 1; }
    }
    if (h[0].first != ((u64)3 << 32) + 7 || h[0].second.m_size != 3 || g_assigns < 8) { std::printf("00a820e0_TEST FAIL\n"); return 1; }
    std::printf("00a820e0_TEST PASS\n");
    return 0;
}