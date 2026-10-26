// Behaviour test for 0x005E3B50: std::__introsort_loop(pair<unsigned long, CAnimationEntry>* first, pair<unsigned long, CAnimationEntry>* last, pair<unsigned long, CAnimationEntry>*, int depth_limit, CKeyPairCompareLess comp) -- STLport introsort recursion over CRC-keyed animation entries: median-of-three partition, recurse on the right part until ranges reach 16 entries; at depth zero fall back to partial_sort
// A copy of the source model and logic (compiled separately from src/005e3b50.cpp) with test stand-ins for the callees.
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <new>
#if !defined(_MSC_VER)
#define __fastcall
#define __forceinline inline
#define __declspec(x)
#define __cdecl
#define __fastcall
#define __int64 long long
#endif

class CAnimComponentMap { public:                                 // intrusively counted (CIVCountedPointeeBase): vptr, count at +4
    virtual void v00();
    virtual void Delete();                                 // slot 1 (+0x04)
    long IVRefCount;                                       // +0x04
};

template <class T> class CIVCountedPointer { public:              // one word: the intrusively counted object
    T* Object;
    CIVCountedPointer(const CIVCountedPointer& other) : Object(other.Object)
    {
        if (Object)
            ++Object->IVRefCount;
    }
    ~CIVCountedPointer()
    {
        T* object = Object;
        if (object) {
            if (--object->IVRefCount == 0)
                object->Delete();
            Object = 0;
        }
    }
};

class CDefString { public:                                        // one word: the def string table position
    long TablePos;
};

class CAnimationEntry {                                    // Ego_r.pdb, 0x10 bytes
public:
    long                                 BankIndex;        // +0x00
    CDefString                           AnimName;         // +0x04
    CDefString                           GroupName;        // +0x08
    CIVCountedPointer<CAnimComponentMap> Components;       // +0x0c
};

namespace std {
template <class _T1, class _T2> struct pair {              // STLport 4.6 pair: explicit memberwise copy constructor
    _T1 first;
    _T2 second;
    pair(const pair<_T1, _T2>& __o) : first(__o.first), second(__o.second) { }
};
}

template <class K, class V> class CKeyPairCompareLess {
public:
    bool operator()(const std::pair<K, V>& a, const std::pair<K, V>& b) const { return a.first < b.first; }
};

typedef std::pair<unsigned long, CAnimationEntry> CAnimPair;
typedef CKeyPairCompareLess<unsigned long, CAnimationEntry> CAnimLess;

namespace std {
template <class _RandomAccessIter, class _Tp, class _Compare>
_RandomAccessIter __fastcall __unguarded_partition(_RandomAccessIter __first, _RandomAccessIter __last, _Tp __pivot, _Compare __comp);   // 0x005E2100
template <class _RandomAccessIter, class _Tp, class _Compare>
void __fastcall __partial_sort(_RandomAccessIter __first, _RandomAccessIter __middle, _RandomAccessIter __last, _Tp*, _Compare __comp);   // 0x005E3550

template <class _Tp>
inline _Tp* __value_type(const _Tp*) { return (_Tp*)0; }

template <class _RandomAccessIter, class _Compare>
inline void partial_sort(_RandomAccessIter __first, _RandomAccessIter __middle, _RandomAccessIter __last, _Compare __comp)
{
    __partial_sort(__first, __middle, __last, __value_type(__first), __comp);
}

template <class _Tp, class _Compare>
inline const _Tp& __median(const _Tp& __a, const _Tp& __b, const _Tp& __c, _Compare __comp)
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

template <class _RandomAccessIter, class _Tp, class _Size, class _Compare>
void __fastcall __introsort_loop(_RandomAccessIter __first, _RandomAccessIter __last, _Tp*, _Size __depth_limit, _Compare __comp)
{
    while (__last - __first > 16) {
        if (__depth_limit == 0) {
            partial_sort(__first, __last, __last, __comp);
            return;
        }
        --__depth_limit;
        _RandomAccessIter __cut =
            __unguarded_partition(__first, __last,
                                  _Tp(__median(*__first, *(__first + (__last - __first) / 2), *(__last - 1), __comp)),
                                  __comp);
        __introsort_loop(__cut, __last, (_Tp*)0, __depth_limit, __comp);
        __last = __cut;
    }
}
}

template void __fastcall std::__introsort_loop<CAnimPair*, CAnimPair, int, CAnimLess>(CAnimPair*, CAnimPair*, CAnimPair*, int, CAnimLess);

// ---- test stand-ins ----
static int g_deletes = 0, g_partialSorts = 0;
void CAnimComponentMap::v00() { }
void CAnimComponentMap::Delete() { ++g_deletes; }

namespace std {
template <class _RandomAccessIter, class _Tp, class _Compare>
_RandomAccessIter __fastcall __unguarded_partition(_RandomAccessIter __first, _RandomAccessIter __last, _Tp __pivot, _Compare __comp)
{                                                         // STLport body, iter_swap through a copy
    for (;;) {
        while (__comp(*__first, __pivot))
            ++__first;
        --__last;
        while (__comp(__pivot, *__last))
            --__last;
        if (!(__first < __last))
            return __first;
        _Tp __tmp = *__first;
        *__first = *__last;
        *__last = __tmp;
        ++__first;
    }
}
template <class _RandomAccessIter, class _Tp, class _Compare>
void __fastcall __partial_sort(_RandomAccessIter __first, _RandomAccessIter, _RandomAccessIter __last, _Tp*, _Compare __comp)
{                                                         // test stand-in: selection sort of the whole range
    ++g_partialSorts;
    for (_RandomAccessIter i = __first; i != __last; ++i)
        for (_RandomAccessIter j = i + 1; j != __last; ++j)
            if (__comp(*j, *i)) { _Tp __tmp = *i; *i = *j; *j = __tmp; }
}
}

static int Fail() { std::printf("005e3b50_TEST FAIL\n"); return 1; }
static void Fill(CAnimPair* p, int n, unsigned long seed, CAnimComponentMap* comp)
{
    for (int i = 0; i < n; ++i) {
        seed = seed * 1103515245u + 12345u;
        p[i].first = (seed >> 8) % 1000u;
        p[i].second.BankIndex = (long)p[i].first + 5;
        p[i].second.AnimName.TablePos = i; p[i].second.GroupName.TablePos = -i;
        p[i].second.Components.Object = (i == 7) ? comp : 0;
    }
}
static void InsertionSort(CAnimPair* p, int n)
{
    for (int i = 1; i < n; ++i)
        for (int j = i; j > 0 && p[j].first < p[j - 1].first; --j) {
            CAnimPair tmp = p[j]; p[j] = p[j - 1]; p[j - 1] = tmp;
        }
}
static bool SortedAndIntact(CAnimPair* p, int n, unsigned long sum)
{
    unsigned long s = 0;
    for (int i = 0; i < n; ++i) {
        if (i && p[i].first < p[i - 1].first) return false;
        if (p[i].second.BankIndex != (long)p[i].first + 5) return false;
        s += p[i].first;
    }
    return s == sum;
}

int main()
{
    CAnimComponentMap comp; comp.IVRefCount = 1;
    const int n = 300;
    CAnimPair* p = (CAnimPair*)std::malloc(n * sizeof(CAnimPair));
    Fill(p, n, 77, &comp);
    unsigned long sum = 0;
    for (int i = 0; i < n; ++i) sum += p[i].first;
    std::__introsort_loop(p, p + n, (CAnimPair*)0, 16, CAnimLess());
    // every element must already sit in a block of at most 16 whose keys are bounded by its neighbours
    InsertionSort(p, n);
    if (!SortedAndIntact(p, n, sum) || g_partialSorts != 0 || comp.IVRefCount != 1 || g_deletes != 0) return Fail();

    Fill(p, 40, 5, &comp);                                  // depth limit 0: falls back to partial_sort at once
    sum = 0;
    for (int i = 0; i < 40; ++i) sum += p[i].first;
    std::__introsort_loop(p, p + 40, (CAnimPair*)0, 0, CAnimLess());
    if (g_partialSorts != 1 || !SortedAndIntact(p, 40, sum)) return Fail();

    Fill(p, 16, 9, &comp);                                  // 16 or fewer: untouched
    unsigned long before = p[0].first, last = p[15].first;
    std::__introsort_loop(p, p + 16, (CAnimPair*)0, 4, CAnimLess());
    if (p[0].first != before || p[15].first != last || g_partialSorts != 1 || comp.IVRefCount != 1) return Fail();
    std::free(p);
    std::printf("005e3b50_TEST PASS\n");
    return 0;
}