// _STL::__find_if(C3DVector* first, C3DVector* last, NShape::CIsPointFinalLinkInTriangleWithMatchingCirculation pred, const random_access_iterator_tag&) -- first point in [first, last) the circulation predicate accepts, else last
// Leaf: ??$__find_if@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00763F50, 212 bytes, `ret 18h`.
// Identity: Ego_r.pdb names the Dinkumware twin ??$_Find_if@PAVC3DVector@@VCIsPointFinalLinkInTriangleWithMatchingCirculation@NShape@@@std@@;
//   retail is STLport's random-access __find_if. Callers: 0x00763100 (NShape triangulation, the predicate built
//   inline as {&a, &b, C3DVector(0, 0, +-1)}) and the out-of-line find_if forwarder 0x007644B0 (`ret 14h`). The
//   catalog's "Polygon_FindFirstPointInside" is an autoname.
// Callee: NShape::CIsPointFinalLinkInTriangleWithMatchingCirculation::operator()(const C3DVector&) const @ 0x0075FE30
//   (seven call sites: four in the unrolled body, three in the remainder switch).
// Shape: STLport 4.x verbatim from an /Gr algorithm TU (__fastcall: first in esi via ecx, last in ebx via edx; the
//   20-byte predicate by value and the tag reference on the stack, `ret 18h`): trip count (last - first) >> 2 with
//   the /12 division as `imul 2AAAAAABh ; sar edx,1`, four tests per pass, then `switch (last - first)` on 3/2/1
//   falling through (`dec eax` chain) and `return last` for 0/default.
struct C3DVector {
    float X, Y, Z;
};

namespace NShape {
struct CIsPointFinalLinkInTriangleWithMatchingCirculation {
    C3DVector* PointA;                            // +0x00
    C3DVector* PointB;                            // +0x04
    C3DVector  UpDirection;                       // +0x08
    bool operator()(const C3DVector& point) const;   // 0x0075FE30
};
}

namespace _STL {

struct input_iterator_tag { };
struct forward_iterator_tag : public input_iterator_tag { };
struct bidirectional_iterator_tag : public forward_iterator_tag { };
struct random_access_iterator_tag : public bidirectional_iterator_tag { };

template <class _RandomAccessIter, class _Predicate>
_RandomAccessIter __fastcall __find_if(_RandomAccessIter __first, _RandomAccessIter __last, _Predicate __pred,
                                       const random_access_iterator_tag&)
{
    int __trip_count = (__last - __first) >> 2;

    for ( ; __trip_count > 0; --__trip_count) {
        if (__pred(*__first)) return __first;
        ++__first;

        if (__pred(*__first)) return __first;
        ++__first;

        if (__pred(*__first)) return __first;
        ++__first;

        if (__pred(*__first)) return __first;
        ++__first;
    }

    switch (__last - __first) {
    case 3:
        if (__pred(*__first)) return __first;
        ++__first;
    case 2:
        if (__pred(*__first)) return __first;
        ++__first;
    case 1:
        if (__pred(*__first)) return __first;
        ++__first;
    case 0:
    default:
        return __last;
    }
}

template C3DVector* __fastcall __find_if<C3DVector*, NShape::CIsPointFinalLinkInTriangleWithMatchingCirculation>(
    C3DVector*, C3DVector*, NShape::CIsPointFinalLinkInTriangleWithMatchingCirculation, const random_access_iterator_tag&);

}