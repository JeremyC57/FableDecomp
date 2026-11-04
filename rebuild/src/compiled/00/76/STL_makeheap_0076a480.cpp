// std::__make_heap(pair<unsigned long, C3DVector>* first, pair<unsigned long, C3DVector>* last, CKeyPairCompareLess<unsigned long, C3DVector> comp, pair<unsigned long, C3DVector>*, int*) -- heapify [first, last) by sifting every parent down, last parent first
// Leaf: ??$__make_heap@
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0076A480, 96 bytes, `ret 0Ch`.
// Callers: 0x0076AB59 and 0x0076B544 (__partial_sort 0x0076B530's inlined make_heap).
// Callee: the landed __adjust_heap 0x00769D50 (same instantiation, first in ecx, hole in edx).
// Shape: STLport `__make_heap(first, last, comp, _Tp*, _Distance*)`, /Gr (__fastcall: first in ecx, last in edx;
//   comp and the two null type pointers on the stack): len = last - first (`sar ..,4`), nothing for len < 2,
//   parent = (len - 2) / 2 (signed `cdq ; sub ; sar`), then `__adjust_heap(first, parent, len, _Tp(first[parent]),
//   comp)` down to parent 0. The comparator is re-read from its slot every pass (all registers busy).
#include "engine/C3DVector.h"

namespace std {

template <class _T1, class _T2>
class pair {
public:
    _T1 first;                                              // +0x00 sort key
    _T2 second;                                             // +0x04
    pair(const pair& o) : first(o.first), second(o.second) { }
};

}

template <class K, class V> class CKeyPairCompareLess { public:
    bool operator()(const std::pair<K, V>& a, const std::pair<K, V>& b) const { return a.first < b.first; }
};

typedef std::pair<unsigned long, C3DVector> SKeyedVector;                 // 16 bytes
typedef CKeyPairCompareLess<unsigned long, C3DVector> SKeyedVectorLess;

namespace std {

template <class _RandomAccessIterator, class _Distance, class _Tp, class _Compare>
void __fastcall __adjust_heap(_RandomAccessIterator __first, _Distance __holeIndex, _Distance __len, _Tp __val, _Compare __comp);   // 0x00769D50

template <class _RandomAccessIterator, class _Compare, class _Tp, class _Distance>
void __fastcall __make_heap(_RandomAccessIterator __first, _RandomAccessIterator __last, _Compare __comp, _Tp*, _Distance*)
{
    if (__last - __first < 2) return;
    _Distance __len = __last - __first;
    _Distance __parent = (__len - 2) / 2;

    for (;;) {
        __adjust_heap(__first, __parent, __len, _Tp(*(__first + __parent)), __comp);
        if (__parent == 0) return;
        __parent--;
    }
}

template void __fastcall __make_heap<SKeyedVector*, SKeyedVectorLess, SKeyedVector, int>(SKeyedVector*, SKeyedVector*, SKeyedVectorLess, SKeyedVector*, int*);

}