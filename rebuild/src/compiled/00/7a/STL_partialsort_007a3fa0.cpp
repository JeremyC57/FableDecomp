// std::__partial_sort(CThing** first, CThing** middle, CThing** last, CThing**, CThingCompare_NearestToPos comp) -- STLport partial sort for the lightning target sort: heap the first part, swap every nearer thing from the rest into it, then heap-sort that part
// Grade: RELOCATION_MATCH (near-miss retry agent901) -- the landed 0x0095DE60 shape with the bool comparator: its real template body (0x007A2F90) noinline in the TU, member-init C3DVector copy ctor, template heap helpers
// Leaf: ??$__partial_sort@PAPAVCThing@@PAV1@UCThingCompare_NearestToPos@@@std@@YIXPAPAVCThing@@000UCThingCompare_NearestToPos@@@Z
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
// Fix (agent901): the shape of the landed 0x0095DE60 (one byte off: its comparator returns a pointer) -- the comparator's
//   real template body (landed 0x007A2F90) noinline in the TU, C3DVector with a member-init copy ctor, template
//   heap helpers declared __fastcall: the register assignment and the dword comparator copies follow retail.
class CThing;

// C3DVector with the engine's constructors (Ego_r.pdb: X, Y, Z): the user copy constructor makes the 12-byte
// comparator copy memberwise into the argument area; operator- / GetLengthSquared serve the stand-in below.
struct C3DVector {
    float X, Y, Z;
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector(const C3DVector& other) : X(other.X), Y(other.Y), Z(other.Z) { }
    C3DVector operator-(const C3DVector& other) const { return C3DVector(X - other.X, Y - other.Y, Z - other.Z); }
    float GetLengthSquared() const { return X * X + Y * Y + Z * Z; }
};

class CThing {
public:
    const C3DVector& GetPosition() const;                 // 0x004C73D0
};

// Ego_r.pdb CThingCompare_Nearest: 12 bytes, Pos (C3DVector) +0x00. Its operator() returns the NEARER of the two
// things (T*, not bool); this TU hands the functor to the STL directly (no CAdaptThingCompareToSTLComparitor),
// so every comparison is `test eax,eax` on the returned pointer.
struct CThingCompare_NearestToPos {                      // 12 bytes, passed by value
    C3DVector Pos;                                       // +0x00
    template <class T> __declspec(noinline) bool operator()(T* a, T* b) const;   // 0x007A2F90 (same-TU body)
};

#pragma auto_inline(off)
template <class T> bool CThingCompare_NearestToPos::operator()(T* a, T* b) const
{
    if (!a)
        return false;
    if (!b)
        return true;
    C3DVector deltaA = a->GetPosition() - Pos;
    C3DVector deltaB = b->GetPosition() - Pos;
    if (deltaA.GetLengthSquared() < deltaB.GetLengthSquared())
        return true;
    return false;
}
#pragma auto_inline(on)
typedef CThingCompare_NearestToPos TComp;

namespace std {
template <class _RandomAccessIterator, class _Distance, class _Tp, class _Compare>
void __fastcall __adjust_heap(_RandomAccessIterator __first, _Distance __holeIndex, _Distance __len, _Tp __val,
                              _Compare __comp);   // 0x007A34A0
template <class _RandomAccessIterator, class _Compare, class _Tp, class _Distance>
void __fastcall __make_heap(_RandomAccessIterator __first, _RandomAccessIterator __last, _Compare __comp, _Tp*,
                            _Distance*);   // 0x007A37B0
template <class _RandomAccessIterator, class _Compare>
void __fastcall sort_heap(_RandomAccessIterator __first, _RandomAccessIterator __last, _Compare __comp);   // 0x007A3B60

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
template void __fastcall __partial_sort<CThing**, CThing*, TComp>(CThing**, CThing**, CThing**, CThing**, TComp);
} // namespace std