#include <cstdio>
#include <cstdlib>
// Behaviour test for 0x00BEE420: _Push_heap over pair<pair<long,long>, CEngineTextureDecal> ordered by the
// lexicographic (long,long) key; the by-value decal frees its blend buffer.
static int g_frees = 0;
struct CCompositeBlendDesc { long m_v; };
template <class T> struct CRetailVector {
    T* _M_start; T* _M_finish; T* _M_end_of_storage;
    CRetailVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
    CRetailVector(const CRetailVector& o) : _M_start(0), _M_finish(0), _M_end_of_storage(0) { *this = o; }
    CRetailVector& operator=(const CRetailVector& o)
    {
        if (this == &o) return *this;
        if (_M_start) { std::free(_M_start); ++g_frees; }
        int n = (int)(o._M_finish - o._M_start);
        _M_start = (T*)std::malloc((n ? n : 1) * sizeof(T));
        for (int i = 0; i < n; ++i) _M_start[i] = o._M_start[i];
        _M_finish = _M_end_of_storage = _M_start + n;
        return *this;
    }
    ~CRetailVector() { if (_M_start) { std::free(_M_start); ++g_frees; } }
};
struct CEngineTextureDecal { long OriginalTextureIndex; long DecalID; CRetailVector<CCompositeBlendDesc> OverlaidBlends; };
template<class K, class V> struct pair { K first; V second; };
template<class K, class V> inline bool operator<(const pair<K, V>& l, const pair<K, V>& r)
{
    return l.first < r.first || !(r.first < l.first) && l.second < r.second;
}
template<class K, class V> struct CKeyPairCompareLess {
    bool operator()(const pair<K, V>& a, const pair<K, V>& b) const { return a.first < b.first; }
    bool operator()(const pair<K, V>& entry, long key0, const K& key) const
    {
        return entry.first.first < key0 || !(key0 < entry.first.first) && entry.first.second < key.second;
    }
};
typedef pair<long, long> Key;
template<class _RanIt, class _Diff, class _Ty, class _Pr>
inline void _Push_heap(_RanIt _First, _Diff _Hole, _Diff _Top, _Ty _Val, _Pr _Pred)
{
    const long _Key0 = _Val.first.first;
    for (_Diff _Idx = (_Hole - 1) / 2; _Hole > _Top && _Pred(*(_First + _Idx), _Key0, _Val.first); _Idx = (_Hole - 1) / 2) {
        *(_First + _Hole) = *(_First + _Idx);
        _Hole = _Idx;
    }
    _RanIt _Dst = _First + _Hole;
    _Dst->first.second = _Val.first.second;
    _Dst->first.first = _Key0;
    _Dst->second = _Val.second;
}
typedef pair<Key, CEngineTextureDecal> Elem;
int main()
{
    int fail = 0;
    {
        Elem h[10]; int n = 0;
        long k0[8] = { 2, 5, 2, -1, 5, 3, 5, 0 };
        long k1[8] = { 9, 1, 10, 4, 3, 3, 2, 7 };
        for (int i = 0; i < 8; ++i) {
            Elem v; v.first.first = k0[i]; v.first.second = k1[i];
            v.second.OriginalTextureIndex = i; v.second.DecalID = 50 + i;
            CCompositeBlendDesc d[2]; d[0].m_v = i; d[1].m_v = -i;
            CRetailVector<CCompositeBlendDesc> src; src._M_start = d; src._M_finish = d + 2; src._M_end_of_storage = d + 2;
            v.second.OverlaidBlends = src; src._M_start = src._M_finish = src._M_end_of_storage = 0;
            int f0 = g_frees;
            _Push_heap(h, n, 0, v, CKeyPairCompareLess<Key, CEngineTextureDecal>()); ++n;
            if (g_frees <= f0) fail = 1;
            for (int c = 1; c < n; ++c)
                if (h[(c - 1) / 2].first < h[c].first) fail = 1;
        }
        if (h[0].first.first != 5 || h[0].first.second != 3 || h[0].second.DecalID != 54 || h[0].second.OverlaidBlends._M_start[1].m_v != -4) fail = 1;
    }
    if (fail) { std::printf("00bee420_TEST FAIL\n"); return 1; }
    std::printf("00bee420_TEST PASS\n");
    return 0;
}