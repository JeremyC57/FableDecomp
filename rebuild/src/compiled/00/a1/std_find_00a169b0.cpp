// std::__find(C2DVector* first, C2DVector* last, const C2DVector& val, const random_access_iterator_tag&) -- STLport random-access find: the first 2D vector in the range equal (within 0.0001 per component) to val, else last
// Leaf: ??$__find@PAVC2DVector@@V1@@std@@YIPAVC2DVector@@PAV1@0ABV1@ABUrandom_access_iterator_tag@0@@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A169B0, 383 bytes, `ret 8` (/Gr __fastcall: first in ecx, last in edx, &val and
//   the tag reference on the stack). Callers: the out-of-line `std::find(first, last, val)` 0x00A16DB0 (fastcall,
//   `lea eax,[esp+4] ; push eax` = the tag in find's own dead argument slot, `ret 4`), CNavigatorManager 0x00A15C0F and
//   CNavigatorAStar 0x00A1DAB0 (inlined find).
// Identity: STLport 4.6 `__find(_RandomAccessIter, _RandomAccessIter, const _Tp&, const random_access_iterator_tag&)`
//   (stl/_algobase.c, verbatim: trip count (last - first) >> 2 = bytes >> 5, four unrolled compares, then the 3/2/1
//   fall-through switch). Element C2DVector (probable): an 8-byte stride of two floats compared with FableWin.pdb's
//   `??8C2DVector@@QBE_NABV0@@Z` shape (|X - v.X| <= 0.0001f && |Y - v.Y| <= 0.0001f); FableWin.pdb also instantiates
//   `std::find<vector<C2DVector>::iterator, C2DVector>`. The manifest label "Vector2_Find" is descriptive.
// Shape: `fld [first] ; fsub [val] ; fabs ; fcomp 0.0001f ; test ah,41h ; jp` per component (`<=`).
#include <math.h>
#pragma intrinsic(fabs)

class C2DVector {
public:
    float X, Y;
    bool operator==(const C2DVector& v) const
    {
        return fabs(X - v.X) <= 0.0001f && fabs(Y - v.Y) <= 0.0001f;
    }
};

namespace std {
struct random_access_iterator_tag { random_access_iterator_tag() {} };
template <class _RandomAccessIter, class _Tp>
_RandomAccessIter __find(_RandomAccessIter __first, _RandomAccessIter __last,
                         const _Tp& __val,
                         const random_access_iterator_tag &)
{
    int __trip_count = (__last - __first) >> 2;

    for ( ; __trip_count > 0 ; --__trip_count) {
        if (*__first == __val) return __first;
        ++__first;

        if (*__first == __val) return __first;
        ++__first;

        if (*__first == __val) return __first;
        ++__first;

        if (*__first == __val) return __first;
        ++__first;
    }

    switch(__last - __first) {
    case 3:
        if (*__first == __val) return __first;
        ++__first;
    case 2:
        if (*__first == __val) return __first;
        ++__first;
    case 1:
        if (*__first == __val) return __first;
        ++__first;
    case 0:
    default:
        return __last;
    }
}
template C2DVector* __find(C2DVector*, C2DVector*, const C2DVector&, const random_access_iterator_tag&);
}