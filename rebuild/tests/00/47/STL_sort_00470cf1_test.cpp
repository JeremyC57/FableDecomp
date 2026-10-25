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
// Model of std::sort<pair<unsigned long,CSoundPair>*, CKeyPairCompareLess<..> > @ 0x00470CF1: introsort with a
// 2 * lg(n) depth budget, then the final insertion sort.

class CSoundPair {                                          // Ego_r.pdb, 8 bytes (a class: mangled V)
public:
    long FirstSound;                                        // +0x0
    long LastSound;                                         // +0x4
};

namespace std {
template <class _T1, class _T2> struct pair {
    _T1 first;                                              // +0x0
    _T2 second;                                             // +0x4
};
}

template <class Key, class Value> class CKeyPairCompareLess {   // empty; passed in a 4-byte slot
public:
    bool operator()(const std::pair<Key, Value>& a, const std::pair<Key, Value>& b) const { return a.first < b.first; }
};

namespace std {
template <class _Size> __forceinline _Size __lg(_Size __n)
{
    _Size __k;
    for (__k = 0; __n != 1; __n >>= 1)
        ++__k;
    return __k;
}

template <class _Tp> __forceinline _Tp* __value_type(const _Tp*) { return (_Tp*)0; }   // _STLP_VALUE_TYPE for pointers

template <class _RandomAccessIter, class _Tp, class _Size, class _Compare>
void __fastcall __introsort_loop(_RandomAccessIter __first, _RandomAccessIter __last, _Tp*, _Size __depth_limit,
                                 _Compare __comp);                                                   // 0x00470E9E
template <class _RandomAccessIter, class _Compare>
void __fastcall __final_insertion_sort(_RandomAccessIter __first, _RandomAccessIter __last, _Compare __comp);   // 0x00470D36

template <class _RandomAccessIter, class _Compare>
void __fastcall sort(_RandomAccessIter __first, _RandomAccessIter __last, _Compare __comp)
{
    if (__first != __last) {
        __introsort_loop(__first, __last, __value_type(__first), __lg(__last - __first) * 2, __comp);
        __final_insertion_sort(__first, __last, __comp);
    }
}

typedef pair<unsigned long, CSoundPair> _SoundEntry;
}

typedef std::pair<unsigned long, CSoundPair> E;
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

namespace TLib {
template <class T> inline T* VT(T*) { return (T*)0; }
template <class RI, class T> inline void IterSwapT(RI a, RI b, T*) { T tmp = *a; *a = *b; *b = tmp; }
template <class RI, class T, class C> void UnguardedLinearInsert(RI last, T val, C comp)
{
    RI next = last;
    --next;
    while (comp(val, *next)) { *last = *next; last = next; --next; }
    *last = val;
}
template <class RI, class T, class C> void LinearInsertT(RI first, RI last, T*, C comp)
{
    T val = *last;
    if (comp(val, *first)) {
        for (RI p = last; p != first; --p)
            *p = *(p - 1);
        *first = val;
    }
    else
        UnguardedLinearInsert(last, val, comp);
}
template <class RI, class C> void InsertionSort(RI first, RI last, C comp)
{
    if (first == last)
        return;
    for (RI i = first + 1; i != last; ++i)
        LinearInsertT(first, i, VT(first), comp);
}
template <class RI, class D, class T, class C> void PushHeap(RI first, D hole, D top, T val, C comp)
{
    D parent = (hole - 1) / 2;
    while (hole > top && comp(*(first + parent), val)) {
        *(first + hole) = *(first + parent);
        hole = parent;
        parent = (hole - 1) / 2;
    }
    *(first + hole) = val;
}
template <class RI, class D, class T, class C> void AdjustHeap(RI first, D hole, D len, T val, C comp)
{
    D top = hole;
    D second = 2 * hole + 2;
    while (second < len) {
        if (comp(*(first + second), *(first + (second - 1))))
            second--;
        *(first + hole) = *(first + second);
        hole = second;
        second = 2 * (second + 1);
    }
    if (second == len) {
        *(first + hole) = *(first + (second - 1));
        hole = second - 1;
    }
    PushHeap(first, hole, top, val, comp);
}
template <class RI, class C, class T, class D> void MakeHeapT(RI first, RI last, C comp, T*, D*)
{
    if (last - first < 2)
        return;
    D len = (D)(last - first);
    D parent = (len - 2) / 2;
    for (;;) {
        T v = *(first + parent);
        AdjustHeap(first, parent, len, v, comp);
        if (parent == 0)
            return;
        parent--;
    }
}
template <class RI, class T, class C> void PopHeapAux(RI first, RI last, T*, C comp)
{
    T val = *(last - 1);
    *(last - 1) = *first;
    AdjustHeap(first, 0, (int)((last - 1) - first), val, comp);
}
template <class RI, class T, class C> void PartialSortT(RI first, RI middle, RI last, T*, C comp)
{
    MakeHeapT(first, middle, comp, (T*)0, (int*)0);
    for (RI i = middle; i < last; ++i)
        if (comp(*i, *first)) {
            T val = *i;
            *i = *first;
            AdjustHeap(first, 0, (int)(middle - first), val, comp);
        }
    while (middle - first > 1) {
        PopHeapAux(first, middle, (T*)0, comp);
        --middle;
    }
}
template <class T, class C> const T& Median(const T& a, const T& b, const T& c, C comp)
{
    if (comp(a, b))
        return comp(b, c) ? b : (comp(a, c) ? c : a);
    return comp(a, c) ? a : (comp(b, c) ? c : b);
}
template <class RI, class T, class C> RI UnguardedPartition(RI first, RI last, T pivot, C comp)
{
    for (;;) {
        while (comp(*first, pivot))
            ++first;
        --last;
        while (comp(pivot, *last))
            --last;
        if (!(first < last))
            return first;
        IterSwapT(first, last, VT(first));
        ++first;
    }
}
template <class RI, class T, class C> void IntrosortLoop(RI first, RI last, T*, int depth, C comp)
{
    while (last - first > 16) {
        if (depth == 0) {
            PartialSortT(first, last, last, (T*)0, comp);
            return;
        }
        --depth;
        T pivot = Median(*first, *(first + (last - first) / 2), *(last - 1), comp);
        RI cut = UnguardedPartition(first, last, pivot, comp);
        IntrosortLoop(cut, last, (T*)0, depth, comp);
        last = cut;
    }
}
}
namespace std {
template <class RI, class T, class S, class C> void __fastcall __introsort_loop(RI first, RI last, T*, S depth, C comp)
{
    TLib::IntrosortLoop(first, last, (T*)0, (int)depth, comp);
}
}
namespace std {
template <class RI, class C> void __fastcall __final_insertion_sort(RI first, RI last, C comp) { TLib::InsertionSort(first, last, comp); }
}
int main()
{
    bool ok = true;
    int sizes[8] = { 0, 1, 2, 5, 16, 17, 40, 150 };
    for (int t = 0; t < 8; ++t) {
        int n = sizes[t];
        E* a = NewEntries(n);
        FillRandom(a, n);
        unsigned long sum = KeySum(a, n);
        std::sort(a, a + n, C());
        if (!SortedOk(a, n) || KeySum(a, n) != sum)
            ok = false;
        free(a);
    }
    if (std::__lg(1) != 0 || std::__lg(16) != 4 || std::__lg(40) != 5)
        ok = false;
    if (!ok) { printf("00470cf1_TEST FAIL\n"); return 1; }
    printf("00470cf1_TEST PASS\n");
    return 0;
}