// std::sort(CThing** first, CThing** last, CThingCompare_Nearest comp) -- entry of the preferred-fire-point sort: introsort to depth 2*lg(n), then the final insertion sort
// Leaf: ??$sort@
// Retail: Fable.exe (TLC, VC7.1) @ 0x0095E010, 111 bytes (+ int3 padding to 112), __fastcall (/Gr algorithm TU: first
//   in ecx, last in edx, the comparator on the stack), `ret 0Ch`.
// Instantiation: the fire-point sort: std::sort<CThing**, CThingCompare_Nearest> called from
//   CAIStateGroup_RangedCombatUsePreferredFirePoint::GetFirePointList (0x0095D4C0) and ::IsRunnable (0x0095D590).
// Callees: __introsort_loop @ 0x0095DF40, __final_insertion_sort @ 0x0095DDF0.
// Shape: STLport `if (first != last) { __introsort_loop(first, last, (T*)0, __lg(last - first) * 2, comp);
//   __final_insertion_sort(first, last, comp); }`; `__lg` is the shift-count loop. The comparator is
//   CThingCompare_Nearest itself, so each test is `test eax,eax` on the nearer thing: the predicate is true whenever
//   either thing is non-null (a retail quirk the sort keeps).
// Label: catalog "std_sort_Introsort" -> the STLport 4.x std::sort instantiation for CThingCompare_Nearest (retail
//   links STLport; the Dinkumware names in Ego_r.pdb are a different build).
// Build: the lane defaults (/O2 /Oy /W3), no pragma; the stand-ins use `#pragma auto_inline(off)` + `__declspec(noinline)`.
class CThing;

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
template <class _Size>
inline _Size __lg(_Size __n)
{
    _Size __k;
    for (__k = 0; __n != 1; __n >>= 1) ++__k;
    return __k;
}
template <class _RandomAccessIter, class _Tp, class _Size, class _Compare>
void __fastcall __introsort_loop(_RandomAccessIter __first, _RandomAccessIter __last, _Tp*, _Size __depth_limit,
                                 _Compare __comp);   // 0x0095DF40
template <class _RandomAccessIter, class _Compare>
void __fastcall __final_insertion_sort(_RandomAccessIter __first, _RandomAccessIter __last, _Compare __comp);   // 0x0095DDF0

template <class _RandomAccessIter, class _Compare>
void __fastcall sort(_RandomAccessIter __first, _RandomAccessIter __last, _Compare __comp)
{
    if (__first != __last) {
        __introsort_loop(__first, __last, (CThing**)0, __lg(__last - __first) * 2, __comp);
        __final_insertion_sort(__first, __last, __comp);
    }
}
template void __fastcall sort<CThing**, TComp>(CThing**, CThing**, TComp);
} // namespace std