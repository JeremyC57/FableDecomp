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
// Model of std::__linear_insert<pair<unsigned long,CDefString>*, ..> @ 0x004712B2: one insertion-sort step (a key below the first shifts the run up,
// otherwise the unguarded walk down from last).

class CDefString {                                          // Ego_r.pdb, 4 bytes (a class: mangled V)
public:
    long TablePos;                                          // +0x0 def-string table index
};

namespace std {
template <class _T1, class _T2> struct pair {
    _T1 first;                                              // +0x0
    _T2 second;                                             // +0x4
    __forceinline pair(const pair<_T1, _T2>& __o) : first(__o.first), second(__o.second) { }
};
}

template <class Key, class Value> class CKeyPairCompareLess {   // empty; passed in a 4-byte slot
public:
    bool operator()(const std::pair<Key, Value>& a, const std::pair<Key, Value>& b) const { return a.first < b.first; }
};

namespace std {
struct __false_type { };

template <class _Tp1, class _Tp2> struct _OKToMemCpy {
    typedef __false_type _Type;
    static _Type _Answer() { return _Type(); }
};
template <class _Tp1, class _Tp2>
__forceinline _OKToMemCpy<_Tp1, _Tp2> _IsOKToMemCpy(_Tp1*, _Tp2*) { return _OKToMemCpy<_Tp1, _Tp2>(); }

template <class _BidirectionalIter1, class _BidirectionalIter2>
_BidirectionalIter2 __fastcall __copy_backward_ptrs(_BidirectionalIter1 __first, _BidirectionalIter1 __last,
                                                    _BidirectionalIter2 __result, const __false_type&);   // 0x0047130E

template <class _Tp1, class _Tp2>
__forceinline _Tp2* copy_backward(_Tp1* __first, _Tp1* __last, _Tp2* __result)
{
    return __copy_backward_ptrs(__first, __last, __result, _IsOKToMemCpy(__first, __result)._Answer());
}

template <class _RandomAccessIter, class _Tp, class _Compare>
__forceinline void __unguarded_linear_insert(_RandomAccessIter __last, _Tp __val, _Compare __comp)
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

template <class _RandomAccessIter, class _Tp, class _Compare>
void __fastcall __linear_insert(_RandomAccessIter __first, _RandomAccessIter __last, _Tp __val, _Compare __comp)
{
    if (__comp(__val, *__first)) {
        copy_backward(__first, __last, __last + 1);
        *__first = __val;
    }
    else
        __unguarded_linear_insert(__last, __val, __comp);
}

typedef pair<unsigned long, CDefString> _CriteriaEntry;
}

typedef std::pair<unsigned long, CDefString> E;
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
namespace std {
template <class A, class B> B __fastcall __copy_backward_ptrs(A first, A last, B result, const __false_type&)
{
    while (first != last)
        *--result = *--last;
    return result;
}
}
int main()
{
    bool ok = true;
    unsigned int base[4] = { 3, 5, 5, 9 };
    unsigned int vals[4] = { 1, 6, 9, 3 };
    unsigned int expect[4][5] = { { 1, 3, 5, 5, 9 }, { 3, 5, 5, 6, 9 }, { 3, 5, 5, 9, 9 }, { 3, 3, 5, 5, 9 } };
    E* a = NewEntries(5);
    for (int t = 0; t < 4; ++t) {
        for (int i = 0; i < 4; ++i)
            SetEntry(a[i], base[i]);
        SetEntry(a[4], vals[t]);
        std::__linear_insert(a, a + 4, E(a[4]), C());
        for (int i = 0; i < 5; ++i)
            if (a[i].first != expect[t][i] || !Consistent(a[i]))
                ok = false;
    }
    if (!ok) { printf("004712b2_TEST FAIL\n"); return 1; }
    printf("004712b2_TEST PASS\n");
    return 0;
}