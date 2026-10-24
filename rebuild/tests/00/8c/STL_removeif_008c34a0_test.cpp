#include <cstdio>
#include <cmath>
#ifndef _MSC_VER
#define __fastcall
#endif
// Model of _STL::remove_if<C3DVector*, CIsOutsideRadius> @ 0x008C34A0: drop the points farther than the radius from the
// centre (full 3D length), compacting the rest to the front; returns the new end.
inline float Sqrtf(float value) { return (float)sqrt((double)value); }
struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector operator-(const C3DVector& o) const { return C3DVector(X - o.X, Y - o.Y, Z - o.Z); }
    float GetLength() const { return Sqrtf(X * X + Y * Y + Z * Z); }
};
struct CIsOutsideRadius {
    float     Radius;
    C3DVector Centre;
    CIsOutsideRadius(float radius, const C3DVector& centre) : Radius(radius), Centre(centre) { }
    CIsOutsideRadius(const CIsOutsideRadius& o) : Radius(o.Radius), Centre(o.Centre) { }
    bool operator()(const C3DVector& p) const
    {
        C3DVector offset = p - Centre;
        return offset.GetLength() > Radius;
    }
};
struct random_access_iterator_tag { };
template <class _RandomAccessIter, class _Predicate>
_RandomAccessIter __fastcall FindIf(_RandomAccessIter __first, _RandomAccessIter __last, _Predicate __pred,
                                    const random_access_iterator_tag&)
{
    int __trip_count = (int)(__last - __first) >> 2;
    for ( ; __trip_count > 0 ; --__trip_count) {
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
template <class _InputIter, class _OutputIter, class _Predicate>
_OutputIter __fastcall RemoveCopyIf(_InputIter __first, _InputIter __last, _OutputIter __result, _Predicate __pred)
{
    for ( ; __first != __last; ++__first)
        if (!__pred(*__first)) {
            *__result = *__first;
            ++__result;
        }
    return __result;
}
template <class _ForwardIter, class _Predicate>
_ForwardIter __fastcall RemoveIf(_ForwardIter __first, _ForwardIter __last, _Predicate __pred)
{
    __first = FindIf(__first, __last, __pred, random_access_iterator_tag());
    if (__first == __last)
        return __first;
    else {
        _ForwardIter __next = __first;
        return RemoveCopyIf(++__next, __last, __first, __pred);
    }
}
int main()
{
    bool ok = true;
    CIsOutsideRadius outside(3.0f, C3DVector(1.0f, 1.0f, 1.0f));
    C3DVector pts[7] = { C3DVector(1, 1, 1), C3DVector(1, 1, 5), C3DVector(2, 2, 2), C3DVector(9, 1, 1),
                         C3DVector(1, 4, 1), C3DVector(1, 1, -3), C3DVector(0, 0, 0) };
    C3DVector* end = RemoveIf(pts, pts + 7, outside);   // (1,1,5) and (9,1,1) and (1,1,-3) are outside
    if (end != pts + 4) ok = false;
    if (pts[0].Z != 1 || pts[1].X != 2 || pts[2].Y != 4 || pts[3].X != 0) ok = false;
    C3DVector none[2] = { C3DVector(1, 1, 1), C3DVector(1, 2, 1) };
    if (RemoveIf(none, none + 2, outside) != none + 2) ok = false;
    if (!ok) { std::printf("008c34a0_TEST FAIL\n"); return 1; }
    std::printf("008c34a0_TEST PASS\n");
    return 0;
}