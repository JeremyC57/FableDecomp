#include <cstdio>
#ifndef _MSC_VER
#define __fastcall
#endif
// Model of the STLport bidirectional __partition<C3DVector*, CIsUsableIciclePoint> @ 0x008E3430: move the points the
// predicate accepts to the front (swapping through a frame temporary), return the partition point.
struct C3DVector {
    float X, Y, Z;
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
};
class CIsUsableIciclePoint {                            // stand-in predicate (the real one is out of line, 0x008E2E40)
public:
    float IciclesLimit;
    bool operator()(const C3DVector& point) const { return point.Z < IciclesLimit; }
};
struct bidirectional_iterator_tag { };
template <class _Tp>
inline void __fastcall Swap(_Tp& __a, _Tp& __b)
{
    _Tp __tmp = __a;
    __a = __b;
    __b = __tmp;
}
template <class _BidirectionalIter, class _Predicate>
_BidirectionalIter __fastcall Partition(_BidirectionalIter __first, _BidirectionalIter __last, _Predicate __pred,
                                        const bidirectional_iterator_tag&)
{
    while (true) {
        while (true)
            if (__first == __last)
                return __first;
            else if (__pred(*__first))
                ++__first;
            else
                break;
        --__last;
        while (true)
            if (__first == __last)
                return __first;
            else if (!__pred(*__last))
                --__last;
            else
                break;
        Swap(*__first, *__last);
        ++__first;
    }
}
int main()
{
    bool ok = true;
    CIsUsableIciclePoint usable;
    usable.IciclesLimit = 5.0f;
    C3DVector pts[7] = { C3DVector(0, 0, 9), C3DVector(1, 0, 1), C3DVector(2, 0, 7), C3DVector(3, 0, 2),
                         C3DVector(4, 0, 8), C3DVector(5, 0, 3), C3DVector(6, 0, 4) };
    C3DVector* mid = Partition(pts, pts + 7, usable, bidirectional_iterator_tag());
    if (mid != pts + 4) ok = false;
    for (int i = 0; i < 7; ++i)
        if ((i < 4) != usable(pts[i])) ok = false;
    if (pts[0].X != 6 || pts[1].X != 1 || pts[2].X != 5 || pts[3].X != 3) ok = false;   // STLport swap order
    if (Partition(pts, pts, usable, bidirectional_iterator_tag()) != pts) ok = false;
    if (!ok) { std::printf("008e3430_TEST FAIL\n"); return 1; }
    std::printf("008e3430_TEST PASS\n");
    return 0;
}