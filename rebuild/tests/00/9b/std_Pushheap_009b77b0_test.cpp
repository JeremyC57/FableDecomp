#include <cstdio>
#include <cstdlib>
// Behaviour test for 0x009B77B0: _Push_heap over pair<unsigned long, CDefClassInfo> (string + alloc func +
// owning CArray<unsigned long>; the by-value argument is destroyed, freeing its array).
static int g_frees = 0, g_strDtors = 0;
struct CCharString {
    int Data;
    CCharString() : Data(0) { }
    CCharString(const CCharString& o) : Data(o.Data) { }
    CCharString& operator=(const CCharString& o) { Data = o.Data; return *this; }
    ~CCharString() { ++g_strDtors; }
};
template<class T> struct CArray {
    T* _M_start; T* _M_finish; T* _M_end_of_storage;
    CArray() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
    CArray(const CArray& o) : _M_start(0), _M_finish(0), _M_end_of_storage(0) { *this = o; }
    CArray& operator=(const CArray& o)
    {
        if (this == &o) return *this;
        if (_M_start) { std::free(_M_start); ++g_frees; }
        int n = (int)(o._M_finish - o._M_start);
        _M_start = n ? (T*)std::malloc(n * sizeof(T)) : 0;
        for (int i = 0; i < n; ++i) _M_start[i] = o._M_start[i];
        _M_finish = _M_end_of_storage = _M_start + n;
        return *this;
    }
    ~CArray() { if (_M_start) { std::free(_M_start); ++g_frees; } }
};
struct CDefClassBase;
static CDefClassBase* AllocA() { return 0; }
struct CDefClassInfo {
    CCharString Name;
    CDefClassBase* (*AllocFunc)();
    CArray<unsigned long> GlobalIndices;
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
typedef pair<unsigned long, CDefClassInfo> Elem;
int main()
{
    int fail = 0;
    {
        Elem h[10]; int n = 0;
        unsigned long keys[7] = { 7, 0xF0000000u, 3, 12, 0xF0000001u, 1, 9 };
        for (int i = 0; i < 7; ++i) {
            Elem v; v.first = keys[i]; v.second.Name.Data = 50 + i; v.second.AllocFunc = AllocA;
            CArray<unsigned long> src; unsigned long buf[2] = { (unsigned long)i, (unsigned long)(i * i) };
            src._M_start = buf; src._M_finish = buf + 2; src._M_end_of_storage = buf + 2;
            v.second.GlobalIndices = src;
            src._M_start = src._M_finish = src._M_end_of_storage = 0;
            int s0 = g_strDtors;
            _Push_heap(h, n, 0, v, CKeyPairCompareLess<unsigned long, CDefClassInfo>()); ++n;
            if (g_strDtors <= s0) fail = 1;
            for (int c = 1; c < n; ++c)
                if (h[(c - 1) / 2].first < h[c].first) fail = 1;
        }
        if (h[0].first != 0xF0000001u || h[0].second.Name.Data != 54 || h[0].second.AllocFunc != AllocA) fail = 1;
        if (h[0].second.GlobalIndices._M_finish - h[0].second.GlobalIndices._M_start != 2 || h[0].second.GlobalIndices._M_start[1] != 16) fail = 1;
    }
    if (fail || g_frees == 0) { std::printf("009b77b0_TEST FAIL\n"); return 1; }
    std::printf("009b77b0_TEST PASS\n");
    return 0;
}