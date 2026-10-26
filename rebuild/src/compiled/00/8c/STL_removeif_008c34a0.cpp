// std::remove_if(C3DVector* first, C3DVector* last, CIsOutsideRadius pred) -- drop the points farther than the radius from the centre (3D), compacting the rest to the front; return the new end
// Leaf: ??$remove_if@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008C34A0, length 100 bytes (+ int3 padding), `ret 10h`.
// Identity: one caller, 0x008C179B in the landed CCombatAction_StrikeResponseJackOfBladesIntoRush::GetValidRushPosition
//   0x008C1600 (the boss phase-start filter there). The predicate is the 16-byte { float Radius; C3DVector Centre }
//   functor of the landed remove_copy_if 0x008C2ED0 (descriptive name CIsOutsideRadius: 3D length > Radius); byte twin
//   of the remove_if 0x008AC060 over unary_negate<CVectorFilter_FurtherFromPointThan>. The catalog label
//   "GetValidRushPosition_impl" is descriptive.
// Callees: the STLport __find_if @ 0x008C2F70 (predicate inlined there) and remove_copy_if @ 0x008C2ED0 (landed).
// Shape: STLport 4.x `remove_if(first, last, pred)` in a /Gr TU (`__fastcall`: first in ecx, last in edx -> esi, the
//   16-byte predicate by value on the stack): find_if inlined to `__find_if(first, last, pred, random_access_iterator_tag())`
//   with the empty tag homed in the live predicate slot (`lea eax,[esp+0Ch]` pushed first), each by-value predicate
//   copied dword by dword (memberwise user copy constructors), then remove_copy_if(++next, last, first, pred).
//   GOTCHAS 24l: the tag lands in the live slot only with __find_if's real body visible (same-TU auto_inline(off) +
//   noinline stand-in below).
extern "C" double __cdecl sqrt(double);
#pragma intrinsic(sqrt)
inline float sqrtf(float value) { return (float)sqrt((double)value); }

class C3DVector { public:
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

namespace std {
struct input_iterator_tag { };
struct forward_iterator_tag : public input_iterator_tag { };
struct bidirectional_iterator_tag : public forward_iterator_tag { };
struct random_access_iterator_tag : public bidirectional_iterator_tag { };

// Same-TU body of the out-of-line __find_if (GOTCHAS 24l: with the body visible VC7.1 homes the empty tag temporary in
// the live predicate argument slot, as retail does).
#pragma auto_inline(off)
template <class _RandomAccessIter, class _Predicate>
__declspec(noinline) _RandomAccessIter __fastcall __find_if(_RandomAccessIter __first, _RandomAccessIter __last, _Predicate __pred,
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
#pragma auto_inline(on)

template <class _InputIter, class _OutputIter, class _Predicate>
_OutputIter __fastcall remove_copy_if(_InputIter __first, _InputIter __last, _OutputIter __result, _Predicate __pred);

template <class _InputIter, class _Predicate>
inline _InputIter find_if(_InputIter __first, _InputIter __last, _Predicate __pred)
{
    return __find_if(__first, __last, __pred, random_access_iterator_tag());
}

template <class _ForwardIter, class _Predicate>
_ForwardIter __fastcall remove_if(_ForwardIter __first, _ForwardIter __last, _Predicate __pred)
{
    __first = find_if(__first, __last, __pred);
    if (__first == __last)
        return __first;
    else {
        _ForwardIter __next = __first;
        return remove_copy_if(++__next, __last, __first, __pred);
    }
}
template C3DVector* __fastcall remove_if<C3DVector*, CIsOutsideRadius>(C3DVector*, C3DVector*, CIsOutsideRadius);
}