// _STL::sort_heap(CThing** first, CThing** last, CThingCompare_NearestToPos comp) -- STLport heap sort for the lightning target sort: pop the nearest-first heap's root to the back until one element remains
// Leaf: sort_heap
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007A3B60, 89 bytes (+ int3 padding to 96), __fastcall (/Gr algorithm TU:
//   first in ecx, last in edx; the 12-byte comparator on the stack), `ret 0Ch`.
// Instantiation: sort<CThing**, CThingCompare_NearestToPos> of CTCWillLightning.cpp (Ego_r.pdb `std::_Sort_heap<CThing**,
//   CThingCompare_NearestToPos>` follows CTCWillLightning::FrameUpdate as this code follows the retail CTCWillLightning
//   block); its one caller is the tail of __partial_sort 0x007A3FA0 (call at 0x007A4040). The catalog's
//   "_Sort_heap<long*>" names the element type wrongly.
// Callees: __adjust_heap 0x007A34A0 (same instantiation, __fastcall: first, holeIndex 0, len, val, comp).
// Layout: CThingCompare_NearestToPos (Ego_r.pdb, 12 bytes) = Pos (C3DVector) +0x00, copied as three dwords; C3DVector
//   has a user copy constructor (a trivial one lets VC hoist the comparator loads out of the loop into ebx / ebp).
// Shape: STLport `while (last - first > 1) pop_heap(first, last--, comp);` with pop_heap / __pop_heap_aux / __pop_heap
//   inline (`val = *(last - 1); *(last - 1) = *first; __adjust_heap(first, 0, (last - 1) - first, val, comp);`); VC
//   strength-reduces `last` into the byte distance `last - first` (eax / edi).
// Label correction: catalogued as `_Sort_heap<long*>`; the body is STLport sort_heap<CThing**, CThingCompare_NearestToPos>.
class CThing;

struct C3DVector {
    float X, Y, Z;
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
};

struct CThingCompare_NearestToPos {                      // 12 bytes, passed by value
    C3DVector Pos;                                       // +0x00
    bool operator()(CThing* a, CThing* b) const;         // 0x007A2F90
};

void __fastcall __adjust_heap(CThing** first, int holeIndex, int len, CThing* val,
                              CThingCompare_NearestToPos comp);   // 0x007A34A0

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
    __pop_heap_aux(__first, __last, (CThing**)0, __comp);
}

void __fastcall sort_heap(CThing** first, CThing** last, CThingCompare_NearestToPos comp)
{
    while (last - first > 1)
        pop_heap(first, last--, comp);
}