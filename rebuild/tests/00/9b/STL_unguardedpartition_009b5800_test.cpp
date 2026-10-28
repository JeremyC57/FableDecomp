// Behaviour test for 0x009B5800 __unguarded_partition over pair<CCharString, long>: names before the cut are <= the pivot, names from it on are >= the pivot; refcounts balance.
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
        const char* n[8] = { "oak", "ash", "yew", "elm", "fir", "box", "pine", "teak" };
        E a[8] = { E(CCharString(n[0]), 0), E(CCharString(n[1]), 1), E(CCharString(n[2]), 2), E(CCharString(n[3]), 3),
                   E(CCharString(n[4]), 4), E(CCharString(n[5]), 5), E(CCharString(n[6]), 6), E(CCharString(n[7]), 7) };
        E pivot(CCharString("fir"), 99);
        E* cut = __unguarded_partition(a, a + 8, E(pivot), C());
        int c = (int)(cut - a);
        if (c <= 0 || c >= 8) FAIL("009b5800")
        for (int i = 0; i < c; ++i) if (pivot.first < a[i].first) FAIL("009b5800")
        for (int i = c; i < 8; ++i) if (a[i].first < pivot.first) FAIL("009b5800")
        for (int i = 0; i < 8; ++i) if (std::strcmp(a[i].first.c(), n[a[i].second]) != 0) FAIL("009b5800")
    }
    if (g_live != 0) FAIL("009b5800")
    std::printf("009b5800_TEST PASS\n");
    return 0;
}