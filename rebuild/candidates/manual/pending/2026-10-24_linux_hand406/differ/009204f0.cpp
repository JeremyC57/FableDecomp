// _STL::__partial_sort(CThing** first, CThing** middle, CThing** last, CThing**, CThingCompare_Nearest comp) -- STLport partial sort for the nearest-first sort: heapify [first, middle), sift every nearer element of [middle, last) in, then sort_heap
// Leaf: ??$__partial_sort@
// Grade: DIFFER(174v172) -- ours copies the comparator's Y/Z floats through the FPU (`fld [comp.Y] ;
//   fld [comp.Z] ; ... fstp [eax+4] ; fstp [eax+8]`, the first pair hoisted to the prologue) where retail moves all three
//   words through ecx/edx; the control flow and calls are identical. Tried: init-list and body-assignment C3DVector copy
//   ctors, no pack(1), a user copy ctor on CThingCompare_Nearest, /Os /Oa /Og /G6 /G7.
// Retail: Fable.exe (TLC, VC7.1) @ 0x009204F0, 172 bytes, __fastcall (first in ecx, middle in edx; last, the value-type
//   tag and the 12-byte comparator on the stack), `ret 14h`.
// Identity: STLport 4.6 __partial_sort<CThing**, CThing*, CThingCompare_Nearest>; callers: __introsort_loop 0x009205D0
//   (depth-limit fallback, 0x009205C1) and partial_sort 0x00920692. The catalog label "Sort_IntroSort" is an autoname.
// Callees: __make_heap<CThing**, CThingCompare_Nearest, CThing*, int> 0x0091FDB0 (two null type tags), CThingCompare_Nearest::
//   operator() 0x0091F740, __adjust_heap 0x0091FB30, sort_heap 0x00920160.
// Family: the STLport sort<CThing**, CThingCompare_Nearest> instantiation of this TU (CAIStateGroup_TestTestTest::Update
//   0x0091F140 sorts its things by distance to the creature, call at 0x0091F2EF); /Gr algorithm TU: every free template is
//   __fastcall (first in ecx, second in edx when it is a pointer/int, the rest -- the 12-byte comparator last -- on the
//   stack). Same family as the landed CThingCompare_NearestToPos sort (0x007A4260 / 0x007A3B60). The comparator's
//   operator()<T> returns the nearer pointer (its catalog label says <CThingMarker>: the template body is identical for
//   every T), tested as a bool (`test eax,eax`).
// Layout: CThingCompare_Nearest (Ego_r.pdb, 12 bytes) = Pos (C3DVector, byte-packed, memberwise user copy ctor: the
//   comparator words are re-read from its argument slot for every by-value copy).
// Shape: the STLport body verbatim: make_heap's inline front end pushes the (T*)0 / (int*)0 tags, the loop compares
//   `comp(*i, *first)` and runs the inline __pop_heap (`*i = *first`, then __adjust_heap(first, 0, middle - first, val));
//   the comparator is copied into the argument area for each out-of-line call.
#pragma pack(push, 1)
class C3DVector {                               // retail: byte-packed, memberwise user copy ctor
public:
    float X, Y, Z;
    C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) { }
};
#pragma pack(pop)

class CThing;

class CThingCompare_Nearest {                   // Ego_r.pdb, 12 bytes, passed by value
public:
    C3DVector Pos;                              // +0x00
    template <class T> T* operator()(T* a, T* b) const;   // 0x0091F740 (returns the nearer thing; null loses)
};

namespace _STL {

template <class _RandomAccessIterator, class _Compare, class _Tp, class _Distance>
void __fastcall __make_heap(_RandomAccessIterator __first, _RandomAccessIterator __last, _Compare __comp, _Tp*,
                            _Distance*);   // 0x0091FDB0

template <class _RandomAccessIterator, class _Distance, class _Tp, class _Compare>
void __fastcall __adjust_heap(_RandomAccessIterator __first, _Distance __holeIndex, _Distance __len, _Tp __value,
                              _Compare __comp);   // 0x0091FB30

template <class _RandomAccessIterator, class _Compare>
void __fastcall sort_heap(_RandomAccessIterator __first, _RandomAccessIterator __last, _Compare __comp);   // 0x00920160

template <class _RandomAccessIterator, class _Compare>
inline void make_heap(_RandomAccessIterator __first, _RandomAccessIterator __last, _Compare __comp)
{
    __make_heap(__first, __last, __comp, (CThing**)0, (int*)0);
}

template <class _RandomAccessIterator, class _Tp, class _Compare, class _Distance>
inline void __pop_heap(_RandomAccessIterator __first, _RandomAccessIterator __last, _RandomAccessIterator __result,
                       _Tp __val, _Compare __comp, _Distance*)
{
    *__result = *__first;
    __adjust_heap(__first, _Distance(0), _Distance(__last - __first), __val, __comp);
}

template <class _RandomAccessIter, class _Tp, class _Compare>
void __fastcall __partial_sort(_RandomAccessIter __first, _RandomAccessIter __middle, _RandomAccessIter __last, _Tp*,
                               _Compare __comp)
{
    make_heap(__first, __middle, __comp);
    for (_RandomAccessIter __i = __middle; __i < __last; ++__i)
        if (__comp(*__i, *__first))
            __pop_heap(__first, __middle, __i, _Tp(*__i), __comp, (int*)0);
    sort_heap(__first, __middle, __comp);
}

template void __fastcall __partial_sort<CThing**, CThing*, CThingCompare_Nearest>(CThing**, CThing**, CThing**, CThing**,
    CThingCompare_Nearest);

}
