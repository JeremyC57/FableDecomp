// Behaviour test for 0x00A41D50: _STL::__introsort_loop over pair<CCharString, 4-byte value> (median-of-three quicksort with the depth fallback)
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <new>
#ifndef _MSC_VER
#define __fastcall
#define __cdecl
#define __forceinline inline
#endif

static int g_fail = 0;
static void Check(bool c) { if (!c) g_fail = 1; }
static bool Near(float a, float b) { float d = a - b; return d < 0.0001f && d > -0.0001f; }

static int g_liveStrings = 0;
class CCharString {                                   // test model: a deep-copying string with a live count
public:
    char* m_pData;
    CCharString() : m_pData(0) { ++g_liveStrings; }
    CCharString(const char* s) : m_pData(Dup(s)) { ++g_liveStrings; }
    CCharString(const CCharString& other) : m_pData(Dup(other.m_pData)) { ++g_liveStrings; }
    ~CCharString() { free(m_pData); --g_liveStrings; }
    CCharString& operator=(const CCharString& other)
    {
        if (this != &other) {
            char* p = Dup(other.m_pData);
            free(m_pData);
            m_pData = p;
        }
        return *this;
    }
    const char* c_str() const { return m_pData ? m_pData : ""; }
    bool operator<(const CCharString& o) const { return strcmp(c_str(), o.c_str()) < 0; }
    static char* Dup(const char* s)
    {
        if (s == 0) return 0;
        char* p = (char*)malloc(strlen(s) + 1);
        strcpy(p, s);
        return p;
    }
};
static bool Eq(const CCharString& s, const char* t) { return strcmp(s.c_str(), t) == 0; }

struct CMapValue4 {
    long m_value;
};

namespace _STL {
template <class _T1, class _T2>
class pair {
public:
    _T1 first;
    _T2 second;
    pair() { }
    pair(const _T1& a, const _T2& b) : first(a), second(b) { }
    pair(const pair& __o) : first(__o.first), second(__o.second) { }
};
}

template <class K, class V>
class CKeyPairCompareLess {
public:
    CKeyPairCompareLess() { }
    bool operator()(const _STL::pair<K, V>& a, const _STL::pair<K, V>& b) const { return a.first < b.first; }
};

typedef _STL::pair<CCharString, CMapValue4> TEntry;
typedef CKeyPairCompareLess<CCharString, CMapValue4> TLess;

namespace _STL {
template <class _Tp>
inline void swap(_Tp& a, _Tp& b) { _Tp t = a; a = b; b = t; }
template <class _Tp, class _Compare>
const _Tp& __fastcall __median(const _Tp& __a, const _Tp& __b, const _Tp& __c, _Compare __comp)
{
    if (__comp(__a, __b))
        if (__comp(__b, __c)) return __b;
        else if (__comp(__a, __c)) return __c;
        else return __a;
    else if (__comp(__a, __c)) return __a;
    else if (__comp(__b, __c)) return __c;
    else return __b;
}
template <class _RandomAccessIter, class _Tp, class _Compare>
_RandomAccessIter __fastcall __unguarded_partition(_RandomAccessIter __first, _RandomAccessIter __last, _Tp __pivot, _Compare __comp)
{
    for ( ; ; ) {
        while (__comp(*__first, __pivot)) ++__first;
        --__last;
        while (__comp(__pivot, *__last)) --__last;
        if (!(__first < __last)) return __first;
        swap(*__first, *__last);
        ++__first;
    }
}
template <class _RandomAccessIter, class _Tp, class _Compare>
void __fastcall __partial_sort(_RandomAccessIter __first, _RandomAccessIter __middle, _RandomAccessIter __last, _Tp*, _Compare __comp)
{
    // test stand-in: selection sort of the prefix
    for (_RandomAccessIter i = __first; i < __middle; ++i) {
        _RandomAccessIter best = i;
        for (_RandomAccessIter j = i + 1; j < __last; ++j)
            if (__comp(*j, *best)) best = j;
        if (best != i) swap(*i, *best);
    }
}
template <class _RandomAccessIter, class _Compare>
inline void partial_sort(_RandomAccessIter __first, _RandomAccessIter __middle, _RandomAccessIter __last, _Compare __comp)
{
    __partial_sort(__first, __middle, __last, (pair<CCharString, CMapValue4>*)0, __comp);
}
template <class _RandomAccessIter, class _Tp, class _Size, class _Compare>
void __fastcall __introsort_loop(_RandomAccessIter __first, _RandomAccessIter __last, _Tp*, _Size __depth_limit, _Compare __comp)
{
    _RandomAccessIter __cut;
    while (__last - __first > 16) {
        if (__depth_limit == 0) {
            partial_sort(__first, __last, __last, __comp);
            return;
        }
        --__depth_limit;
        __cut = __unguarded_partition(__first, __last,
                                      _Tp(__median(*__first, *(__first + (__last - __first) / 2), *(__last - 1), __comp)),
                                      __comp);
        __introsort_loop(__cut, __last, (_Tp*)0, __depth_limit, __comp);
        __last = __cut;
    }
}
}

static void Fill(TEntry* a, int n, unsigned int seed)
{
    for (int i = 0; i < n; ++i) {
        seed = seed * 1103515245u + 12345u;
        unsigned int k = (seed >> 8) % 1000u;
        char name[8];
        sprintf(name, "k%03u", k);
        a[i].first = CCharString(name);
        a[i].second.m_value = (long)k;
    }
}

static void Finish(TEntry* a, int n)   // the final insertion sort std::sort runs after the loop
{
    for (int i = 1; i < n; ++i)
        for (int j = i; j > 0 && a[j].first < a[j - 1].first; --j)
            _STL::swap(a[j], a[j - 1]);
}

static bool SortedAndConsistent(const TEntry* a, int n, long expectSum)
{
    long sum = 0;
    for (int i = 0; i < n; ++i) {
        char name[8];
        sprintf(name, "k%03u", (unsigned int)a[i].second.m_value);
        if (!Eq(a[i].first, name)) return false;
        if (i > 0 && a[i].first < a[i - 1].first) return false;
        sum += a[i].second.m_value;
    }
    return sum == expectSum;
}

int main()
{
    {
        const int n = 60;
        TEntry a[n];
        Fill(a, n, 7u);
        long sum = 0;
        for (int i = 0; i < n; ++i) sum += a[i].second.m_value;
        int live = g_liveStrings;
        _STL::__introsort_loop(a, a + n, (TEntry*)0, 12, TLess());
        Check(g_liveStrings == live);
        // every run of at most 16 elements left unsorted is bounded by its neighbours: the final insertion sort finishes it
        Finish(a, n);
        Check(SortedAndConsistent(a, n, sum));
        // depth limit 0: the whole range goes through partial_sort
        Fill(a, n, 99u);
        sum = 0;
        for (int j = 0; j < n; ++j) sum += a[j].second.m_value;
        _STL::__introsort_loop(a, a + n, (TEntry*)0, 0, TLess());
        Check(SortedAndConsistent(a, n, sum));
        // 16 or fewer: untouched
        Fill(a, 16, 5u);
        TEntry b[16];
        for (int k = 0; k < 16; ++k) b[k] = a[k];
        _STL::__introsort_loop(a, a + 16, (TEntry*)0, 12, TLess());
        for (int m = 0; m < 16; ++m) Check(Eq(a[m].first, b[m].first.c_str()) && a[m].second.m_value == b[m].second.m_value);
    }
    Check(g_liveStrings == 0);
    if (g_fail) {
        printf("00a41d50_TEST FAIL\n");
        return 1;
    }
    printf("00a41d50_TEST PASS\n");
    return 0;
}