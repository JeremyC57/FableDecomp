#include <cstdio>
// Behaviour test for 0x00992940: _Push_heap over pair<long, CSoundBankRecord> (non-trivial record:
// user operator=, destructor run on the by-value argument).
static int g_assigns = 0, g_dtors = 0;
struct CSoundBankRecord {
    int m_id;
    CSoundBankRecord() : m_id(0) { }
    CSoundBankRecord(const CSoundBankRecord& other) : m_id(other.m_id) { }
    CSoundBankRecord& operator=(const CSoundBankRecord& other) { m_id = other.m_id; ++g_assigns; return *this; }
    ~CSoundBankRecord() { ++g_dtors; }
};
template<class K, class V> struct pair { K first; V second; };
template<class K, class V> class CKeyPairCompareLess { public:
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
typedef pair<long, CSoundBankRecord> Elem;
int main()
{
    Elem h[10]; int n = 0;
    long keys[8] = { 3, -2, 8, 5, -9, 8, 10, 1 };
    for (int i = 0; i < 8; ++i) {
        Elem v; v.first = keys[i]; v.second.m_id = 100 + i;
        int d0 = g_dtors;
        _Push_heap(h, n, 0, v, CKeyPairCompareLess<long, CSoundBankRecord>()); ++n;
        if (g_dtors <= d0) { std::printf("00992940_TEST FAIL\n"); return 1; }
        for (int c = 1; c < n; ++c)
            if (h[(c - 1) / 2].first < h[c].first) { std::printf("00992940_TEST FAIL\n"); return 1; }
    }
    if (h[0].first != 10 || h[0].second.m_id != 106 || g_assigns < 8) { std::printf("00992940_TEST FAIL\n"); return 1; }
    std::printf("00992940_TEST PASS\n");
    return 0;
}