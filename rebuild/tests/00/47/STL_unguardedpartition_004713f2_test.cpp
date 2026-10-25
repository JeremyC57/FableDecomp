#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <new>
#ifndef _MSC_VER
#define __fastcall
#define __cdecl
#define __forceinline inline
#define __declspec(x)
#endif
// Model of _STL::__unguarded_partition<pair<unsigned long,CDefString>*, ..> @ 0x004713F2: Hoare partition around a
// by-value pivot taken from the range.

class CDefString {                                          // Ego_r.pdb, 4 bytes (a class: mangled V)
public:
    long TablePos;                                          // +0x0 def-string table index
};

namespace _STL {
template <class _T1, class _T2> struct pair {
    _T1 first;                                              // +0x0
    _T2 second;                                             // +0x4
    __forceinline pair(const pair<_T1, _T2>& __o) : first(__o.first), second(__o.second) { }
};
}

template <class Key, class Value> class CKeyPairCompareLess {   // empty; passed in a 4-byte slot
public:
    bool operator()(const _STL::pair<Key, Value>& a, const _STL::pair<Key, Value>& b) const { return a.first < b.first; }
};

namespace _STL {
template <class _ForwardIter1, class _ForwardIter2, class _Tp>
__forceinline void __iter_swap(_ForwardIter1 __a, _ForwardIter2 __b, _Tp*)
{
    _Tp __tmp = *__a;
    *__a = *__b;
    *__b = __tmp;
}
template <class _Tp> __forceinline _Tp* __value_type(const _Tp*) { return (_Tp*)0; }   // _STLP_VALUE_TYPE for pointers
template <class _ForwardIter1, class _ForwardIter2>
__forceinline void iter_swap(_ForwardIter1 __i1, _ForwardIter2 __i2)
{
    __iter_swap(__i1, __i2, __value_type(__i1));
}

template <class _RandomAccessIter, class _Tp, class _Compare>
_RandomAccessIter __fastcall __unguarded_partition(_RandomAccessIter __first, _RandomAccessIter __last, _Tp __pivot,
                                                   _Compare __comp)
{
    for (;;) {
        while (__comp(*__first, __pivot))
            ++__first;
        --__last;
        while (__comp(__pivot, *__last))
            --__last;
        if (!(__first < __last))
            return __first;
        iter_swap(__first, __last);
        ++__first;
    }
}

typedef pair<unsigned long, CDefString> _CriteriaEntry;
}

typedef _STL::pair<unsigned long, CDefString> E;
typedef CKeyPairCompareLess<unsigned long, CDefString> C;
static void SetEntry(E& e, unsigned int key) { e.first = key; e.second.TablePos = (long)(key * 5); }
static bool Consistent(const E& e) { return e.second.TablePos == (long)(e.first * 5); }
static unsigned int gSeed = 12345u;
static unsigned int NextKey() { gSeed = gSeed * 1103515245u + 12345u; return (gSeed >> 16) % 50u; }
static E* NewEntries(int n) { return (E*)malloc((n + 1) * sizeof(E)); }
static void FillRandom(E* a, int n) { for (int i = 0; i < n; ++i) SetEntry(a[i], NextKey()); }
static unsigned long KeySum(const E* a, int n) { unsigned long s = 0; for (int i = 0; i < n; ++i) s += a[i].first; return s; }
static bool SortedOk(const E* a, int n)
{
    for (int i = 0; i < n; ++i)
        if (!Consistent(a[i]) || (i > 0 && a[i - 1].first > a[i].first))
            return false;
    return true;
}
int main()
{
    bool ok = true;
    for (int t = 0; t < 20; ++t) {
        int n = 2 + t * 3;
        E* a = NewEntries(n);
        FillRandom(a, n);
        unsigned long sum = KeySum(a, n);
        E pivot(a[n / 2]);
        E* cut = _STL::__unguarded_partition(a, a + n, pivot, C());
        if (cut < a || cut > a + n)
            ok = false;
        for (E* p = a; p < cut; ++p)
            if (p->first > pivot.first)
                ok = false;
        for (E* p = cut; p < a + n; ++p)
            if (p->first < pivot.first)
                ok = false;
        for (int i = 0; i < n; ++i)
            if (!Consistent(a[i]))
                ok = false;
        if (KeySum(a, n) != sum)
            ok = false;
        free(a);
    }
    if (!ok) { printf("004713f2_TEST FAIL\n"); return 1; }
    printf("004713f2_TEST PASS\n");
    return 0;
}