// std::__find(const CNavigationPosition* first, const CNavigationPosition* last, const C2DVector& val, const random_access_iterator_tag&) -- STLport four-way unrolled linear search of the action-point positions for the first one within 1e-4 of val in X and Y; last when none
// Leaf: ??$__find@PBVCNavigationPosition@@VC2DVector@@@std@@YIPBVCNavigationPosition@@PBV1@0ABVC2DVector@@ABUrandom_access_iterator_tag@0@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A7EB00, 412 bytes, /Gr `__fastcall` (first ecx, last edx; the value and the tag
//   references on the stack), `ret 8`.
// Identity (probable): its caller 0x00A77540 sits right before CNavQuadTree::IsActionPointReachable 0x00A77570, matching
//   Ego_r.pdb's CNavQuadTree::IsActionPoint(const C2DVector&) const 0xBBD3C0 / IsActionPointReachable 0xBBD3E0 donor order;
//   it searches [this+0x50, this+0x54) = CNavQuadTree::ActionPoints (vector<CNavigationPosition>, Ego_r 0x60 less the
//   three earlier Dinkumware vectors and the CVectorMap) for the C2DVector argument it passes straight through. The
//   landed caller binds this address under an invented `FindPosition(C3DVector*, ...)` spelling.
// Callees: none; the epsilon is the 0.0001f pool constant at 0x0129BA3C.
// Shape: STLport 4.6 _algo.c `__find` (random-access: trip count = n >> 2, four tests per pass, then the switch on the
//   remainder 3/2/1 falling through) with the position comparison inline: |X - v.X| <= 1e-4 && |Y - v.Y| <= 1e-4.
extern "C" double __cdecl fabs(double x);
#pragma intrinsic(fabs)

class C2DVector {
public:
    float X, Y;
};
class CNavigationPosition {                                // 12 bytes (Ego_r.pdb)
public:
    C2DVector Position;                                    // +0x00
    long      Layer;                                       // +0x08
    bool operator==(const C2DVector& p) const
    {
        return (float)fabs(Position.X - p.X) <= 0.0001f && (float)fabs(Position.Y - p.Y) <= 0.0001f;
    }
};

namespace std {
struct random_access_iterator_tag { random_access_iterator_tag() { } };
template <class _RandomAccessIter, class _Tp>
_RandomAccessIter __fastcall __find(_RandomAccessIter __first, _RandomAccessIter __last, const _Tp& __val,
                                    const random_access_iterator_tag&)
{
    int __trip_count = (__last - __first) >> 2;

    for (; __trip_count > 0; --__trip_count) {
        if (*__first == __val) return __first;
        ++__first;

        if (*__first == __val) return __first;
        ++__first;

        if (*__first == __val) return __first;
        ++__first;

        if (*__first == __val) return __first;
        ++__first;
    }

    switch (__last - __first) {
    case 3:
        if (*__first == __val) return __first;
        ++__first;
    case 2:
        if (*__first == __val) return __first;
        ++__first;
    case 1:
        if (*__first == __val) return __first;
    case 0:
    default:
        return __last;
    }
}
template const CNavigationPosition* __fastcall __find(const CNavigationPosition*, const CNavigationPosition*, const C2DVector&, const random_access_iterator_tag&);
}