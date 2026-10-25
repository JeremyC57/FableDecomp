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
// Model of _STL::__copy_backward_ptrs<pair<unsigned long,CSoundPair>*, ..> @ 0x00470E6D: counted backward copy that
// returns the new start of the destination (safe for an overlapping shift up).

class CSoundPair {                                          // Ego_r.pdb, 8 bytes (a class: mangled V)
public:
    long FirstSound;                                        // +0x0
    long LastSound;                                         // +0x4
};

namespace _STL {
struct random_access_iterator_tag { random_access_iterator_tag() { } };
struct __false_type { __false_type() { } };

template <class _T1, class _T2> struct pair {
    _T1 first;                                              // +0x0
    _T2 second;                                             // +0x4
};

template <class _RandomAccessIter, class _BidirectionalIter, class _Distance>
__forceinline _BidirectionalIter __copy_backward(_RandomAccessIter __first, _RandomAccessIter __last, _BidirectionalIter __result,
                                                 const random_access_iterator_tag&, _Distance*)
{
    for (_Distance __n = __last - __first; __n > 0; --__n)
        *--__result = *--__last;
    return __result;
}

template <class _BidirectionalIter1, class _BidirectionalIter2>
_BidirectionalIter2 __fastcall __copy_backward_ptrs(_BidirectionalIter1 __first, _BidirectionalIter1 __last,
                                                    _BidirectionalIter2 __result, const __false_type&)
{
    return __copy_backward(__first, __last, __result, random_access_iterator_tag(), (int*)0);
}

typedef pair<unsigned long, CSoundPair> _SoundEntry;
}

template <class Key, class Value> class CKeyPairCompareLess {   // (test only: the row has no comparator)
public:
    bool operator()(const _STL::pair<Key, Value>& a, const _STL::pair<Key, Value>& b) const { return a.first < b.first; }
};

typedef _STL::pair<unsigned long, CSoundPair> E;
typedef CKeyPairCompareLess<unsigned long, CSoundPair> C;
static void SetEntry(E& e, unsigned int key) { e.first = key; e.second.FirstSound = (long)(key * 3); e.second.LastSound = (long)(key + 7); }
static bool Consistent(const E& e) { return e.second.FirstSound == (long)(e.first * 3) && e.second.LastSound == (long)(e.first + 7); }
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
    E* a = NewEntries(6);
    for (int i = 0; i < 6; ++i)
        SetEntry(a[i], i + 1);
    E* r = _STL::__copy_backward_ptrs(a, a + 3, a + 5, _STL::__false_type());
    unsigned int expect[6] = { 1, 2, 1, 2, 3, 6 };
    for (int i = 0; i < 6; ++i)
        if (a[i].first != expect[i] || !Consistent(a[i]))
            ok = false;
    if (r != a + 2)
        ok = false;
    if (_STL::__copy_backward_ptrs(a, a, a + 4, _STL::__false_type()) != a + 4)
        ok = false;
    if (!ok) { printf("00470e6d_TEST FAIL\n"); return 1; }
    printf("00470e6d_TEST PASS\n");
    return 0;
}