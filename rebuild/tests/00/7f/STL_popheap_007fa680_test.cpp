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
// Model of std::__pop_heap<CThing**, CThing*, CThingCompare_BestForTargeting2, int> @ 0x007FA680: moves the heap top
// to *result and sifts the saved value into the shortened heap; repeated, it heap-sorts the targets.
class CThing { public: int Score; };
class C3DVector { public: float X, Y, Z; C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { } };

class CVectorCompare_BestForTargeting { public:                        // Ego_r.pdb, 44 bytes
    C3DVector AnchorPos;                                        // +0x00
    C3DVector PreferredLine;                                    // +0x0c
    float     AngleCompare;                                     // +0x18
    float     SqrDistCompare;                                   // +0x1c
    float     OverrideShortDistanceFactor;                      // +0x20
    float     OverrideShortDistanceFalloff;                     // +0x24
    bool      CompareInXYPlane;                                 // +0x28
};

class CThingCompare_BestForTargeting2 {                         // Ego_r.pdb, 52 bytes
public:
    CThing* Hero;                                               // +0x00
    CVectorCompare_BestForTargeting VectorCompare;              // +0x04
    float EnemyPreferenceFactor;
    bool operator()(const CThing* a, const CThing* b) const { return a->Score < b->Score; }   // test ordering                                // +0x30
};

namespace std {
template <class _RandomAccessIterator, class _Distance, class _Tp, class _Compare>
void __fastcall __adjust_heap(_RandomAccessIterator __first, _Distance __holeIndex, _Distance __len, _Tp __value, _Compare __comp);   // 0x007FA320

template <class _RandomAccessIterator, class _Tp, class _Compare, class _Distance>
void __fastcall __pop_heap(_RandomAccessIterator __first, _RandomAccessIterator __last, _RandomAccessIterator __result,
                           _Tp __value, _Compare __comp, _Distance*)
{
    *__result = *__first;
    __adjust_heap(__first, _Distance(0), _Distance(__last - __first), __value, __comp);
}

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
template <class RI, class D, class T, class C> void __fastcall __adjust_heap(RI first, D hole, D len, T val, C comp)
{
    TLib::AdjustHeap(first, hole, len, val, comp);
}
}
int main()
{
    bool ok = true;
    static CThing things[7];
    int scores[7] = { 5, 9, 1, 7, 3, 8, 2 };
    CThing* a[7];
    for (int i = 0; i < 7; ++i) {
        things[i].Score = scores[i];
        a[i] = &things[i];
    }
    union { double align; unsigned char bytes[sizeof(CThingCompare_BestForTargeting2)]; } raw;
    memset(raw.bytes, 0, sizeof(raw.bytes));
    const CThingCompare_BestForTargeting2& comp = *(const CThingCompare_BestForTargeting2*)raw.bytes;
    TLib::MakeHeapT(a, a + 7, comp, (CThing**)0, (int*)0);
    for (CThing** last = a + 7; last - a > 1; --last)
        std::__pop_heap(a, last - 1, last - 1, *(last - 1), comp, (int*)0);
    for (int i = 1; i < 7; ++i)
        if (a[i - 1]->Score > a[i]->Score)
            ok = false;
    if (a[0]->Score != 1 || a[6]->Score != 9)
        ok = false;
    if (!ok) { printf("007fa680_TEST FAIL\n"); return 1; }
    printf("007fa680_TEST PASS\n");
    return 0;
}