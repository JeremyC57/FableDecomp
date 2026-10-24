// std::remove_copy_if(C3DVector* first, C3DVector* last, C3DVector* result, CIsOutsideRadius pred) -- copy the points within the radius of the centre to result, return the new end
// Leaf: remove_copy_if
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008C2ED0, length 116 bytes (__fastcall first, last; result and the 16-byte predicate on the stack).
// Identity: the catalog label Geometry::FilterPointsWithinRadius names the caller's purpose; the body is the SGI/STLport
//   remove_copy_if instance its remove_if (0x008C34A0) calls.
// Shape: the length is `sqrtf(X*X + Y*Y + Z*Z)` through an inline float sqrtf(float) wrapper over the sqrt intrinsic (a double
//   expression reorders the x87 stack); the predicate names its `offset = p - Centre` local; the predicate has a memberwise user copy
//   constructor and C3DVector a 3-float constructor and operator-.
extern "C" double __cdecl sqrt(double);
#pragma intrinsic(sqrt)
inline float sqrtf(float value) { return (float)sqrt((double)value); }
struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector operator-(const C3DVector& o) const { return C3DVector(X - o.X, Y - o.Y, Z - o.Z); }
    float GetLength() const { return sqrtf(X * X + Y * Y + Z * Z); }
};
struct CIsOutsideRadius {
    float     Radius;
    C3DVector Centre;
    CIsOutsideRadius(const CIsOutsideRadius& o) : Radius(o.Radius), Centre(o.Centre) { }
    bool operator()(const C3DVector& p) const
    {
        C3DVector offset = p - Centre;
        return offset.GetLength() > Radius;
    }
};
namespace std {
template <class _InputIter, class _OutputIter, class _Predicate>
_OutputIter __fastcall remove_copy_if(_InputIter __first, _InputIter __last, _OutputIter __result, _Predicate __pred)
{
    for ( ; __first != __last; ++__first)
        if (!__pred(*__first)) {
            *__result = *__first;
            ++__result;
        }
    return __result;
}
template C3DVector* __fastcall remove_copy_if<C3DVector*, C3DVector*, CIsOutsideRadius>(C3DVector*, C3DVector*, C3DVector*, CIsOutsideRadius);
}