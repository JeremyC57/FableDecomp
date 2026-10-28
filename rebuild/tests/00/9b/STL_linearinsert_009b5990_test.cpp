// Behaviour test for 0x009B5990 __linear_insert over pair<CCharString, long>: a new minimum name shifts the prefix up and lands first, any other goes unguarded; refcounts balance.
#include <cstdio>
#include <cstring>
static int g_live = 0;
class CCharStringData { public: char Text[16]; long NoRefs; };
class CCharString {                                 // ref-counted model; a null block is the empty string
public:
    CCharStringData* PStringData;
    CCharString(const char* s) : PStringData(0) { ++g_live; if (s && *s) { PStringData = new CCharStringData; std::strcpy(PStringData->Text, s); PStringData->NoRefs = 1; } }
    CCharString(const CCharString& o) : PStringData(o.PStringData) { ++g_live; if (PStringData) ++PStringData->NoRefs; }
    ~CCharString() { Release(); --g_live; }
    void Release() { if (PStringData && --PStringData->NoRefs == 0) delete PStringData; PStringData = 0; }
    const CCharString& operator=(const CCharString& o) { if (o.PStringData) ++o.PStringData->NoRefs; Release(); PStringData = o.PStringData; return *this; }
    bool operator<(const CCharString& other) const
    {
        if (other.PStringData == PStringData) return false;
        if (PStringData == 0) return true;
        if (other.PStringData == 0) return false;
        return std::strcmp(PStringData->Text, other.PStringData->Text) < 0;
    }
    const char* c() const { return PStringData ? PStringData->Text : ""; }
};
template <class T1, class T2> struct pair {
    T1 first; T2 second;
    pair(const T1& a, const T2& b) : first(a), second(b) {}
    pair(const pair<T1, T2>& o) : first(o.first), second(o.second) {}
};
template <class K, class V> class CKeyPairCompareLess { public:
    bool operator()(const pair<K, V>& a, const pair<K, V>& b) const { return a.first < b.first; }
};
typedef pair<CCharString, int> E;
typedef CKeyPairCompareLess<CCharString, int> C;
template <class _RI, class _Tp, class _Compare>
void __unguarded_linear_insert(_RI __last, _Tp __val, _Compare __comp)
{
    _RI __next = __last; --__next;
    while (__comp(__val, *__next)) { *__last = *__next; __last = __next; --__next; }
    *__last = __val;
}
template <class _RI, class _Tp, class _Compare>
void __linear_insert(_RI __first, _RI __last, _Tp __val, _Compare __comp)
{
    if (__comp(__val, *__first)) {
        _RI __result = __last + 1;
        for (int __n = (int)(__last - __first); __n > 0; --__n) *--__result = *--__last;
        *__first = __val;
    }
    else __unguarded_linear_insert(__last, __val, __comp);
}
template <class _RI, class _Tp, class _Compare>
_RI __unguarded_partition(_RI __first, _RI __last, _Tp __pivot, _Compare __comp)
{
    for (;;) {
        while (__comp(*__first, __pivot)) ++__first;
        --__last;
        while (__comp(__pivot, *__last)) --__last;
        if (!(__first < __last)) return __first;
        _Tp __tmp = *__first; *__first = *__last; *__last = __tmp;
        ++__first;
    }
}
#define FAIL(a) { std::printf(a "_TEST FAIL\n"); return 1; }
int main()
{
    {
        E a[4] = { E(CCharString("fig"), 1), E(CCharString("lime"), 2), E(CCharString("plum"), 3), E(CCharString("date"), 4) };
        __linear_insert(a, a + 3, E(a[3]), C());
        const char* ek[4] = { "date", "fig", "lime", "plum" }; int ev[4] = { 4, 1, 2, 3 };
        for (int i = 0; i < 4; ++i) if (std::strcmp(a[i].first.c(), ek[i]) != 0 || a[i].second != ev[i]) FAIL("009b5990")
        E b[3] = { E(CCharString("b"), 1), E(CCharString("d"), 2), E(CCharString("c"), 3) };
        __linear_insert(b, b + 2, E(b[2]), C());
        if (std::strcmp(b[1].first.c(), "c") != 0 || b[1].second != 3 || std::strcmp(b[2].first.c(), "d") != 0) FAIL("009b5990")
        for (int i = 0; i < 4; ++i) if (a[i].first.PStringData->NoRefs != 1) FAIL("009b5990")
    }
    if (g_live != 0) FAIL("009b5990")
    std::printf("009b5990_TEST PASS\n");
    return 0;
}