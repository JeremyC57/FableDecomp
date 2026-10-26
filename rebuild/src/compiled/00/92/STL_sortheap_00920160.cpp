// std::sort_heap(const CThingObject** first, const CThingObject** last, CThingCompare_Nearest comp) -- STLport heap sort for the nearest-first sort: pop the heap's root to the back until one element remains
// Leaf: ??$sort_heap@
// Element (sweep3, FINDINGS "Family pass 2" G015): the sorted range is const CThingObject* -- the caller
//   CAIStateGroup_TestTestTest::Update 0x0091F140 fills it with GetAllThingsInRoughArea 0x00920200 (type mask 0x807 =
//   CThingObject) before sort 0x009206A0, and FableWin.pdb has sort<vector<const CThingObject*>, CThingCompare_Nearest> /
//   sort_heap<const CThingObject**, CThingCompare_Nearest>; the comparator 0x0091F740 is therefore
//   operator()<const CThingObject> (??$?R$$CBVCThingObject@@@CThingCompare_Nearest@@...), not operator()<CThing> (0x004A8D80).
// Retail: Fable.exe (TLC, VC7.1) @ 0x00920160, 89 bytes, __fastcall (first in ecx, last in edx; the 12-byte comparator on
//   the stack), `ret 0Ch`.
// Identity: STLport 4.6 sort_heap<const CThingObject**, CThingCompare_Nearest>; single caller __partial_sort 0x009204F0 (0x00920590).
//   The catalog label "_Sort_heap<long*>" names the element type wrongly (as at the landed twin 0x007A3B60).
// Callees: __adjust_heap<const CThingObject**, int, const CThingObject*, CThingCompare_Nearest> 0x0091FB30.
// Family: the STLport sort<const CThingObject**, CThingCompare_Nearest> instantiation of this TU (CAIStateGroup_TestTestTest::Update
//   0x0091F140 sorts its things by distance to the creature, call at 0x0091F2EF); /Gr algorithm TU: every free template is
//   __fastcall (first in ecx, second in edx when it is a pointer/int, the rest -- the 12-byte comparator last -- on the
//   stack). Same family as the landed CThingCompare_NearestToPos sort (0x007A4260 / 0x007A3B60). The comparator's
//   operator()<T> returns the nearer pointer (its catalog label says <CThingMarker>: the template body is identical for
//   every T), tested as a bool (`test eax,eax`).
// Layout: CThingCompare_Nearest (Ego_r.pdb, 12 bytes) = Pos (C3DVector, byte-packed, memberwise user copy ctor: the
//   comparator words are re-read from its argument slot for every by-value copy).
// Shape: the landed 0x007A3B60 body: `while (last - first > 1) pop_heap(first, last--, comp);` with pop_heap /
//   __pop_heap_aux / __pop_heap inline; VC strength-reduces `last` into the byte distance.
#pragma pack(push, 1)
class C3DVector {                               // retail: byte-packed, memberwise user copy ctor
public:
    float X, Y, Z;
    C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) { }
};
#pragma pack(pop)

class CThingObject;

class CThingCompare_Nearest {                   // Ego_r.pdb, 12 bytes, passed by value
public:
    C3DVector Pos;                              // +0x00
    template <class T> T* operator()(T* a, T* b) const;   // 0x0091F740 (returns the nearer thing; null loses)
};

namespace std {

template <class _RandomAccessIterator, class _Distance, class _Tp, class _Compare>
void __fastcall __adjust_heap(_RandomAccessIterator __first, _Distance __holeIndex, _Distance __len, _Tp __value,
                              _Compare __comp);   // 0x0091FB30

template <class _RandomAccessIterator, class _Tp, class _Compare, class _Distance>
inline void __pop_heap(_RandomAccessIterator __first, _RandomAccessIterator __last, _RandomAccessIterator __result,
                       _Tp __val, _Compare __comp, _Distance*)
{
    *__result = *__first;
    __adjust_heap(__first, _Distance(0), _Distance(__last - __first), __val, __comp);
}

template <class _RandomAccessIterator, class _Tp, class _Compare>
inline void __pop_heap_aux(_RandomAccessIterator __first, _RandomAccessIterator __last, _Tp*, _Compare __comp)
{
    __pop_heap(__first, __last - 1, __last - 1, _Tp(*(__last - 1)), __comp, (int*)0);
}

template <class _RandomAccessIterator, class _Compare>
inline void pop_heap(_RandomAccessIterator __first, _RandomAccessIterator __last, _Compare __comp)
{
    __pop_heap_aux(__first, __last, (const CThingObject**)0, __comp);
}

template <class _RandomAccessIterator, class _Compare>
void __fastcall sort_heap(_RandomAccessIterator __first, _RandomAccessIterator __last, _Compare __comp)
{
    while (__last - __first > 1)
        pop_heap(__first, __last--, __comp);
}

template void __fastcall sort_heap<const CThingObject**, CThingCompare_Nearest>(const CThingObject**, const CThingObject**, CThingCompare_Nearest);

}