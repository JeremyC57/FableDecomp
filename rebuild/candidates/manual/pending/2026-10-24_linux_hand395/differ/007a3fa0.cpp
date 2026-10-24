// _STL::__partial_sort(CThing** first, CThing** middle, CThing** last, CThing**, CThingCompare_NearestToPos comp) -- STLport partial sort for the lightning target sort: heap the first part, swap every nearer thing from the rest into it, then heap-sort that part
// Grade: DIFFER(172v172) -- register allocation: with the trivial comparator copy, first lands in edi and i in esi (retail esi / edi) and the pop step's loads rotate (val in ecx, *first through edx); the C3DVector copy-ctor form (as the matched sort_heap) copies Y/Z through fld/fstp (174v172); dword members 184v172
// Leaf: __partial_sort
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007A3FA0, 172 bytes (+ int3 padding to 176), __fastcall (/Gr algorithm TU:
//   first in ecx, middle in edx; last, the value-type tag pointer and the 12-byte comparator on the stack), `ret 14h`.
// Instantiation: sort / partial_sort<CThing**, CThingCompare_NearestToPos> of CTCWillLightning.cpp (Ego_r.pdb places the
//   CThingCompare_NearestToPos sort family straight after CTCWillLightning::FrameUpdate); callers partial_sort
//   0x007A40D0 and __introsort_loop 0x007A4140 (depth-limit fallback). The catalog's "PartialSort_Top_CThingAICreature"
//   names the element type wrongly.
// Callees: __make_heap 0x007A37B0, CThingCompare_NearestToPos::operator()<CThing>(CThing*, CThing*) const 0x007A2F90
//   (thiscall on the comparator; landed under the CThingCompare_Nearest<CThingAICreature> label), __adjust_heap
//   0x007A34A0, sort_heap 0x007A3B60 (all the same instantiation, __fastcall).
// Layout: CThingCompare_NearestToPos (Ego_r.pdb, 12 bytes) = Pos (C3DVector) +0x00, copied as three dwords (trivial copy
//   here; the matched sibling sort_heap 0x007A3B60 needs a C3DVector user copy constructor, which in this body copies Y / Z
//   through the x87 stack).
// Shape: STLport `make_heap(first, middle, comp); for (i = middle; i < last; ++i) if (comp(*i, *first))
//   __pop_heap(first, middle, i, _Tp(*i), comp, (int*)0); sort_heap(first, middle, comp);` -- make_heap and __pop_heap
//   inline (`push 0` x2 for the tag pointers; `*i = *first; __adjust_heap(first, 0, middle - first, val, comp)`).
class CThing;

struct C3DVector {
    float X, Y, Z;
};

struct CThingCompare_NearestToPos {                      // 12 bytes, passed by value
    C3DVector Pos;                                       // +0x00
    bool operator()(CThing* a, CThing* b) const;         // 0x007A2F90
};

void __fastcall __make_heap(CThing** first, CThing** last, CThingCompare_NearestToPos comp, CThing**, int*);   // 0x007A37B0
void __fastcall __adjust_heap(CThing** first, int holeIndex, int len, CThing* val,
                              CThingCompare_NearestToPos comp);   // 0x007A34A0
void __fastcall sort_heap(CThing** first, CThing** last, CThingCompare_NearestToPos comp);   // 0x007A3B60

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

void __fastcall __partial_sort(CThing** first, CThing** middle, CThing** last, CThing**, CThingCompare_NearestToPos comp)
{
    make_heap(first, middle, comp);
    for (CThing** i = middle; i < last; ++i)
        if (comp(*i, *first))
            __pop_heap(first, middle, i, (CThing*)(*i), comp, (int*)0);
    sort_heap(first, middle, comp);
}
