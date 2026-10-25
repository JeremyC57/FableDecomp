#include <cstdio>
#ifndef _MSC_VER
#define __fastcall
#endif
// Model of std::remove_if<C3DVector*, unary_negate<CVectorFilter_FurtherFromPointThan> > @ 0x008AC060: find the first
// point within the XY radius, then compact the remaining far points over it; returns the new end.
struct C3DVector {
    float X, Y, Z;
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
};
struct C2DVector {
    float X, Y;
    C2DVector(float x, float y) : X(x), Y(y) { }
    float GetLengthSquared() const { return X * X + Y * Y; }
};
class CVectorFilter_FurtherFromPointThan {
public:
    float     Radius;
    C3DVector CentrePos;
    CVectorFilter_FurtherFromPointThan(float radius, const C3DVector& centre) : Radius(radius), CentrePos(centre) { }
    bool operator()(const C3DVector& pos) const
    {
        C2DVector delta(pos.X - CentrePos.X, pos.Y - CentrePos.Y);
        float radius = Radius;
        return delta.GetLengthSquared() > radius * radius;
    }
};
template <class _Predicate>
class unary_negate {
protected:
    _Predicate _M_pred;
public:
    explicit unary_negate(const _Predicate& __x) : _M_pred(__x) { }
    bool operator()(const C3DVector& __x) const { return !_M_pred(__x); }
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
    CVectorFilter_FurtherFromPointThan further(2.0f, C3DVector(0.0f, 0.0f, 0.0f));
    unary_negate<CVectorFilter_FurtherFromPointThan> within(further);
    for (int n = 0; n <= 9; ++n) {                     // lengths around the four-way unroll
        C3DVector pts[9] = { C3DVector(5, 0, 0), C3DVector(1, 0, 0), C3DVector(0, 6, 0), C3DVector(0, 1, 9),
                             C3DVector(-7, 0, 0), C3DVector(1, 1, 0), C3DVector(3, 3, 0), C3DVector(0, 0, 0),
                             C3DVector(0, -4, 0) };
        C3DVector* end = RemoveIf(pts, pts + n, within);
        int kept = 0;
        float expectX[9] = { 5, 0, -7, 3, 0 };
        float expectY[9] = { 0, 6, 0, 3, -4 };
        int far = 0;
        for (int i = 0; i < n; ++i) {
            static const bool isFar[9] = { true, false, true, false, true, false, true, false, true };
            if (isFar[i]) ++far;
        }
        for (C3DVector* p = pts; p != end; ++p, ++kept)
            if (p->X != expectX[kept] || p->Y != expectY[kept]) ok = false;
        if (kept != far) ok = false;
    }
    if (!ok) { std::printf("008ac060_TEST FAIL\n"); return 1; }
    std::printf("008ac060_TEST PASS\n");
    return 0;
}