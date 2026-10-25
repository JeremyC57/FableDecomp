// std::sort_heap(CThing** first, CThing** last, CThingCompare_Nearest comp) -- heap-sort tail of the preferred-fire-point sort: pop the heap root to the back until one element remains
// Leaf: ??$sort_heap@
// Retail: Fable.exe (TLC, VC7.1) @ 0x0095DB30, 89 bytes (+ int3 padding to 96), __fastcall (/Gr algorithm TU: first
//   in ecx, last in edx, the comparator on the stack), `ret 0Ch`.
// Instantiation: the fire-point sort: std::sort<CThing**, CThingCompare_Nearest> called from
//   CAIStateGroup_RangedCombatUsePreferredFirePoint::GetFirePointList (0x0095D4C0) and ::IsRunnable (0x0095D590).
// Callees: __adjust_heap @ 0x0095CFE0.
// Shape: STLport `while (last - first > 1) pop_heap(first, last--, comp);` with pop_heap / __pop_heap_aux /
//   __pop_heap inline. The comparator is CThingCompare_Nearest itself, so each test is `test eax,eax` on the nearer
//   thing: the predicate is true whenever either thing is non-null (a retail quirk the sort keeps).
// Label: catalog "_Sort_heap<long*>" -> the STLport 4.x std::sort_heap instantiation for CThingCompare_Nearest
//   (retail links STLport; the Dinkumware names in Ego_r.pdb are a different build).
// Build: the lane defaults (/O2 /Oy /W3), no pragma; the stand-ins use `#pragma auto_inline(off)` + `__declspec(noinline)`.
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
class CThingCompare_Nearest {
public:
    C3DVector Pos;                                        // +0x00
    template <class T> __declspec(noinline) T* operator()(T* a, T* b) const;   // 0x004A8D80
};

// Same-TU stand-in for the template operator() (retail 0x004A8D80, folded with the other T = CThing copies; outside
// these bytes): with the body in the TU VC7.1 knows the call keeps the comparator intact and pipelines its copies.
#pragma auto_inline(off)
template <class T> T* CThingCompare_Nearest::operator()(T* a, T* b) const
{
    if (!a)
        return b;
    if (!b)
        return a;
    C3DVector deltaA = a->GetPosition() - Pos;
    C3DVector deltaB = b->GetPosition() - Pos;
    return deltaA.GetLengthSquared() < deltaB.GetLengthSquared() ? a : b;
}
#pragma auto_inline(on)
typedef CThingCompare_Nearest TComp;

namespace std {
template <class _RandomAccessIterator, class _Distance, class _Tp, class _Compare>
void __fastcall __adjust_heap(_RandomAccessIterator __first, _Distance __holeIndex, _Distance __len, _Tp __val,
                              _Compare __comp);   // 0x0095CFE0

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
template <class _RandomAccessIterator, class _Compare>
void __fastcall sort_heap(_RandomAccessIterator __first, _RandomAccessIterator __last, _Compare __comp)
{
    while (__last - __first > 1)
        pop_heap(__first, __last--, __comp);
}
template void __fastcall sort_heap<CThing**, TComp>(CThing**, CThing**, TComp);
} // namespace std