// std::__push_heap(CThing** first, int holeIndex, int topIndex, CThing* val, CThingCompare_Nearest comp) -- STLport heap sift-up: while the hole is below the top and comp(parent, val) holds, move the parent down; then store val in the hole
// Leaf: ??$__push_heap@
// Retail: Fable.exe (TLC, VC7.1) @ 0x0095CDF0, 97 bytes, /Gr __fastcall (first ecx, holeIndex edx; topIndex, val and the
//   comparator on the stack), `ret 14h`.
// Identity (proven): only caller the landed std::__adjust_heap<CThing**, int, CThing*, CThingCompare_Nearest> 0x0095CFE0 (the preferred-fire-point sort; it names this callee at 0x0095CDF0); comparator CThingCompare_Nearest::operator()<CThing> 0x004A8D80.
// Shape (family K019, representative 0x00748880): STLport 4.6 _heap.c `parent = (hole - 1) / 2; while (hole > top &&
//   comp(*(first + parent), val)) { *(first + hole) = *(first + parent); hole = parent; parent = (hole - 1) / 2; }
//   *(first + hole) = val;` (SGI `hole > top`, signed divides by 2 through cdq/sub/sar).
// C3DVector with the engine's constructors (Ego_r.pdb: X, Y, Z): the user copy constructor makes the 12-byte
// comparator copy memberwise into the argument area; operator- / GetLengthSquared serve the stand-in below.
class C3DVector { public:
    float X, Y, Z;
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector(const C3DVector& other) : X(other.X), Y(other.Y), Z(other.Z) { }
    C3DVector operator-(const C3DVector& other) const { return C3DVector(X - other.X, Y - other.Y, Z - other.Z); }
    float GetLengthSquared() const { return X * X + Y * Y + Z * Z; }
};
class CThing {
public:
    const C3DVector& GetPos() const;                 // 0x004C73D0
};

// Ego_r.pdb CThingCompare_Nearest: 12 bytes, Pos (C3DVector) +0x00. Its operator() returns the NEARER of the two
// things (T*, not bool), so the heap test is `test eax,eax` on the returned pointer.
class CThingCompare_Nearest {
public:
    C3DVector Pos;                                        // +0x00
    template <class T> __declspec(noinline) T* operator()(T* a, T* b) const;   // 0x004A8D80
};
// Same-TU stand-in for the template operator() (retail 0x004A8D80, outside these bytes), as the landed sibling
// __adjust_heap 0x0095CFE0 compiles it: VC7.1 then knows the call keeps the comparator intact.
#pragma auto_inline(off)
template <class T> T* CThingCompare_Nearest::operator()(T* a, T* b) const
{
    if (!a)
        return b;
    if (!b)
        return a;
    C3DVector deltaA = a->GetPos() - Pos;
    C3DVector deltaB = b->GetPos() - Pos;
    return deltaA.GetLengthSquared() < deltaB.GetLengthSquared() ? a : b;
}
#pragma auto_inline(on)
typedef CThingCompare_Nearest TComp;

namespace std {
template <class _RandomAccessIterator, class _Distance, class _Tp, class _Compare>
void __fastcall __push_heap(_RandomAccessIterator __first, _Distance __holeIndex, _Distance __topIndex, _Tp __val,
                            _Compare __comp)
{
    _Distance __parent = (__holeIndex - 1) / 2;
    while (__holeIndex > __topIndex && __comp(*(__first + __parent), __val)) {
        *(__first + __holeIndex) = *(__first + __parent);
        __holeIndex = __parent;
        __parent = (__holeIndex - 1) / 2;
    }
    *(__first + __holeIndex) = __val;
}
template void __fastcall __push_heap<CThing**, int, CThing*, TComp>(CThing**, int, int, CThing*, TComp);
} // namespace std