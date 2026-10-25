// _STL::__find_if(C3DVector* first, C3DVector* last, CIsOutsideRadius pred, const random_access_iterator_tag&) -- STLport's four-way unrolled random-access find_if: the first point farther (3D) than the radius from the centre, or `last`
// Leaf: ??$__find_if@
// Retail: Fable.exe (TLC, VC7.1) @ 0x008C2F70, 518 bytes, `__fastcall` (first in ecx, last in edx), `ret 14h` (the
//   16-byte predicate by value + the tag reference).
// Label: the catalog name Geometry::FindClosestPointInRadius is descriptive; the landed remove_if 0x008C34A0 (its caller,
//   together with remove_copy_if 0x008C2ED0) names it "the STLport __find_if @ 0x008C2F70 (predicate inlined there)" and
//   carries this body as its same-TU stand-in.
// Shape: STLport 4.x `__find_if` for random-access iterators in a /Gr TU; the predicate is the landed CIsOutsideRadius
//   ({ float Radius; C3DVector Centre }, `(p - Centre).GetLength() > Radius`).
extern "C" double __cdecl sqrt(double);
#pragma intrinsic(sqrt)
inline float sqrtf(float value) { return (float)sqrt((double)value); }

struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector operator-(const C3DVector& o) const { return C3DVector(X - o.X, Y - o.Y, Z - o.Z); }
    float GetLength() const { return sqrtf(X * X + Y * Y + Z * Z); }
};

struct CIsOutsideRadius {                       // 16 bytes (descriptive name, as the landed 0x008C2ED0)
    float     Radius;                           // +0x00
    C3DVector Centre;                           // +0x04
    CIsOutsideRadius(const CIsOutsideRadius& o) : Radius(o.Radius), Centre(o.Centre) { }
    bool operator()(const C3DVector& p) const
    {
        C3DVector offset = p - Centre;
        return offset.GetLength() > Radius;
    }
};

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
template C3DVector* __fastcall __find_if<C3DVector*, CIsOutsideRadius>(C3DVector*, C3DVector*, CIsOutsideRadius,
                                                                       const random_access_iterator_tag&);
}