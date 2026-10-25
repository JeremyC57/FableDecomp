// std::__adjust_heap(CThing** first, int holeIndex, int len, CThing* val, CThingCompare_Nearest comp) -- heap sift-down of the preferred-fire-point sort: move the hole to a leaf along the better children, then sift val back up
// Leaf: ??$__adjust_heap@
// Retail: Fable.exe (TLC, VC7.1) @ 0x0095CFE0, 135 bytes (+ int3 padding to 144), __fastcall (/Gr algorithm TU: first
//   in ecx, holeIndex in edx, len, val and the comparator on the stack), `ret 14h`.
// Instantiation: the fire-point sort: std::sort<CThing**, CThingCompare_Nearest> called from
//   CAIStateGroup_RangedCombatUsePreferredFirePoint::GetFirePointList (0x0095D4C0) and ::IsRunnable (0x0095D590).
// Callees: CThingCompare_Nearest::operator()<CThing> @ 0x004A8D80 (the returned pointer is the predicate),
//   __push_heap @ 0x0095CDF0.
// Shape: STLport 4.x `__adjust_heap` (secondChild = 2*hole + 2, the lone last child, then `__push_heap(first, hole,
//   top, val, comp)` out of line). The comparator is CThingCompare_Nearest itself, so each test is `test eax,eax` on
//   the nearer thing: the predicate is true whenever either thing is non-null (a retail quirk the sort keeps).
// Label: catalog "_Adjust_heap<CThing**,int,CThing*,CThingCompare_Nearest>" -> the STLport 4.x std::__adjust_heap
//   instantiation for CThingCompare_Nearest (retail links STLport; the Dinkumware names in Ego_r.pdb are a different
//   build).
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
void __fastcall __push_heap(_RandomAccessIterator __first, _Distance __holeIndex, _Distance __topIndex, _Tp __val,
                            _Compare __comp);   // 0x0095CDF0

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