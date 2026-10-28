// std::__partial_sort(C3DSkeletalBoneMorph* first, C3DSkeletalBoneMorph* middle, C3DSkeletalBoneMorph* last, C3DSkeletalBoneMorph*, bool (__fastcall* comp)(const C3DSkeletalBoneMorph&, const C3DSkeletalBoneMorph&)) -- SGI partial sort: heapify [first, middle), pop every smaller element of [middle, last) into the heap, then sort_heap
// Leaf: ??$__partial_sort@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A46940, length 124 bytes, `ret 0Ch`.
// Callers: 0x00A46A8C (partial_sort) and 0x00A46F23 (the introsort loop's depth-limit fallback).
// Callees: 0x00A46350 __make_heap (this chunk), 0x00A462A0 __pop_heap (landed), 0x00A466F0 sort_heap (landed),
//   CCharString copy ctor 0x0099EC30 (the by-value element argument built in the argument area).
// Element (Ego_r.pdb, 20 bytes): C3DSkeletalBoneMorph = BoneName CCharString +0, BoneNameCRC +4, Scaling C3DVector +8.
// Label correction: catalogued as `Introsort_Partition_With_Predicate`.
// Shape: SGI/STLport `__partial_sort` verbatim in a /Gr TU (`__fastcall`: first in ecx -> ebp, middle in edx -> ebx;
//   last, the null value-type pointer and the comparator on the stack, the comparator kept in edi and re-read after
//   each __pop_heap because edi addresses the argument area); `make_heap(first, middle, comp)` inlines to
//   `__make_heap(first, middle, comp, (T*)0, (int*)0)`; the loop is `for (i = middle; i < last; ++i)`.
class CCharString {
public:
    void* m_pData;                                   // +0x00
    CCharString(const CCharString& other);           // 0x0099EC30
    ~CCharString();                                  // 0x0099EAE0
    const CCharString& operator=(const CCharString& other);   // 0x0099EFB0
};

class C3DVector {                                    // retail C3DVector: memberwise user copy ctor, implicit operator=
public:
    float X, Y, Z;
    C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) { }
};

class C3DSkeletalBoneMorph {
public:
    CCharString   BoneName;                          // +0x00
    unsigned long BoneNameCRC;                       // +0x04
    C3DVector     Scaling;                           // +0x08
};

typedef bool (__fastcall* TBoneMorphCompare)(const C3DSkeletalBoneMorph& a, const C3DSkeletalBoneMorph& b);

namespace std {
template <class _RandomAccessIterator, class _Compare, class _Tp, class _Distance>
void __fastcall __make_heap(_RandomAccessIterator __first, _RandomAccessIterator __last, _Compare __comp, _Tp*, _Distance*);   // 0x00A46350

template <class _RandomAccessIterator, class _Compare>
inline void __fastcall make_heap(_RandomAccessIterator __first, _RandomAccessIterator __last, _Compare __comp)
{
    __make_heap(__first, __last, __comp, (C3DSkeletalBoneMorph*)0, (int*)0);
}

template <class _RandomAccessIterator, class _Tp, class _Compare, class _Distance>
void __fastcall __pop_heap(_RandomAccessIterator __first, _RandomAccessIterator __last, _RandomAccessIterator __result,
                           _Tp __val, _Compare __comp, _Distance*);   // 0x00A462A0

template <class _RandomAccessIterator, class _Compare>
void __fastcall sort_heap(_RandomAccessIterator __first, _RandomAccessIterator __last, _Compare __comp);   // 0x00A466F0

template <class _RandomAccessIter, class _Tp, class _Compare>
void __fastcall __partial_sort(_RandomAccessIter __first, _RandomAccessIter __middle, _RandomAccessIter __last, _Tp*, _Compare __comp)
{
    make_heap(__first, __middle, __comp);
    for (_RandomAccessIter __i = __middle; __i < __last; ++__i)
        if (__comp(*__i, *__first))
            __pop_heap(__first, __middle, __i, _Tp(*__i), __comp, (int*)0);
    sort_heap(__first, __middle, __comp);
}

template void __fastcall __partial_sort(C3DSkeletalBoneMorph*, C3DSkeletalBoneMorph*, C3DSkeletalBoneMorph*, C3DSkeletalBoneMorph*, TBoneMorphCompare);
}