// std::__adjust_heap(CThing** first, int holeIndex, int len, CThing* val, CThingCompare_NearestToPos comp) -- STLport heap sift-down for the lightning target sort: move the nearer child up until the hole reaches the bottom, then push `val` back up from there
// Grade: RELOCATION_MATCH (near-miss retry agent901) -- the landed 0x0095CFE0 shape with the bool comparator: its real template body (0x007A2F90) noinline in the TU, member-init C3DVector copy ctor, template __adjust_heap
// Leaf: ??$__adjust_heap@PAPAVCThing@@HPAV1@UCThingCompare_NearestToPos@@@std@@YIXPAPAVCThing@@HHPAV1@UCThingCompare_NearestToPos@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007A34A0, 135 bytes, __fastcall (/Gr algorithm TU: first in ecx, holeIndex
//   in edx; len, val and the 12-byte comparator on the stack), `ret 14h`.
// Instantiation: sort / partial_sort<CThing**, CThingCompare_NearestToPos> of CTCWillLightning.cpp -- Ego_r.pdb places
//   `CThingCompare_NearestToPos::operator()<CThing>` and the std::_Push_heap / _Adjust_heap / ... / _Sort<CThing**,
//   CThingCompare_NearestToPos> family straight after CTCWillLightning::FrameUpdate, the same position this code holds in
//   retail (after the CTCWillLightning block, before its GetInterfaceType); callers __make_heap 0x007A3740.., __partial_sort
//   0x007A3FA0 and __sort_heap 0x007A3B60 of the same family. The catalog name is the Ego_r spelling.
// Callees: CThingCompare_NearestToPos::operator()<CThing>(CThing*, CThing*) const 0x007A2F90 (thiscall on the comparator;
//   landed under the CThingCompare_Nearest<CThingAICreature> label), __push_heap 0x007A30B0 (same instantiation, __fastcall).
// Layout: CThingCompare_NearestToPos (Ego_r.pdb, 12 bytes) = Pos (C3DVector) +0x00, copied as three dwords.
// Shape: STLport `topIndex = holeIndex; secondChild = 2 * holeIndex + 2; while (secondChild < len) { if
//   (comp(*(first + secondChild), *(first + (secondChild - 1)))) secondChild--; *(first + holeIndex) = *(first +
//   secondChild); holeIndex = secondChild; secondChild = 2 * (secondChild + 1); } if (secondChild == len) { ... }
//   __push_heap(first, holeIndex, topIndex, val, comp);` -- topIndex lives in the `push ecx` frame word; the comparator
//   copy goes through eax only with a C3DVector user copy constructor (the trivial copy uses ecx).
// Fix (agent901): the shape of the landed 0x0095CFE0 (one byte off: its comparator returns a pointer) -- the comparator's
//   real template body (landed 0x007A2F90) noinline in the TU, C3DVector with a member-init copy ctor, a template
//   __adjust_heap: retail's X/Y-before-`sub esp,0Ch` copy schedule follows.
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
void __fastcall __push_heap(_RandomAccessIterator __first, _Distance __holeIndex, _Distance __topIndex, _Tp __val,
                            _Compare __comp);   // 0x007A30B0

template <class _RandomAccessIterator, class _Distance, class _Tp, class _Compare>
void __fastcall __adjust_heap(_RandomAccessIterator __first, _Distance __holeIndex, _Distance __len, _Tp __val,
                              _Compare __comp)
{
    _Distance __topIndex = __holeIndex;
    _Distance __secondChild = 2 * __holeIndex + 2;
    while (__secondChild < __len) {
        if (__comp(*(__first + __secondChild), *(__first + (__secondChild - 1))))
            __secondChild--;
        *(__first + __holeIndex) = *(__first + __secondChild);
        __holeIndex = __secondChild;
        __secondChild = 2 * (__secondChild + 1);
    }
    if (__secondChild == __len) {
        *(__first + __holeIndex) = *(__first + (__secondChild - 1));
        __holeIndex = __secondChild - 1;
    }
    __push_heap(__first, __holeIndex, __topIndex, __val, __comp);
}
template void __fastcall __adjust_heap<CThing**, int, CThing*, TComp>(CThing**, int, int, CThing*, TComp);
} // namespace std