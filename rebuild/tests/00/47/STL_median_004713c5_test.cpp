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
// Model of _STL::__median<pair<unsigned long,CDefString>, ..> @ 0x004713C5: the median of three entries by key
// (a reference to one of the arguments).

class CDefString {                                          // Ego_r.pdb, 4 bytes (a class: mangled V)
public:
    long TablePos;                                          // +0x0 def-string table index
};

namespace _STL {
template <class _T1, class _T2> struct pair {
    _T1 first;                                              // +0x0
    _T2 second;                                             // +0x4
};
}

template <class Key, class Value> class CKeyPairCompareLess {   // empty; passed in a 4-byte slot
public:
    bool operator()(const _STL::pair<Key, Value>& a, const _STL::pair<Key, Value>& b) const { return a.first < b.first; }
};

namespace _STL {
template <class _Tp, class _Compare>
const _Tp& __fastcall __median(const _Tp& __a, const _Tp& __b, const _Tp& __c, _Compare __comp)
{
    if (__comp(__a, __b))
        if (__comp(__b, __c))
            return __b;
        else if (__comp(__a, __c))
            return __c;
        else
            return __a;
    else if (__comp(__a, __c))
        return __a;
    else if (__comp(__b, __c))
        return __c;
    else
        return __b;
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
    unsigned int perms[9][3] = { { 1, 2, 3 }, { 1, 3, 2 }, { 2, 1, 3 }, { 2, 3, 1 }, { 3, 1, 2 }, { 3, 2, 1 },
                                 { 2, 2, 1 }, { 1, 2, 2 }, { 2, 1, 2 } };
    E* a = NewEntries(3);
    for (int t = 0; t < 9; ++t) {
        for (int i = 0; i < 3; ++i)
            SetEntry(a[i], perms[t][i]);
        const E& m = _STL::__median(a[0], a[1], a[2], C());
        if (m.first != 2 || (&m != &a[0] && &m != &a[1] && &m != &a[2]))
            ok = false;
    }
    if (!ok) { printf("004713c5_TEST FAIL\n"); return 1; }
    printf("004713c5_TEST PASS\n");
    return 0;
}