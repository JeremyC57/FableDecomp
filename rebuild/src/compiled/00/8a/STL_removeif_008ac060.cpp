// std::remove_if(C3DVector* first, C3DVector* last, unary_negate<CVectorFilter_FurtherFromPointThan> pred) -- drop the points within the XY radius of the centre, compacting the rest to the front; return the new end
// Leaf: ??$remove_if@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008AC060, length 100 bytes (+ int3 padding), `ret 10h`.
// Identity: FableWin.pdb names `remove_if` / `_Find_if` / `_Remove_if` over unary_negate<CVectorFilter_FurtherFromPointThan>
//   (Ego_r.pdb: { float Radius; C3DVector CentrePos }, 16 bytes); seven callers (0x008A947B, 0x008C1758, 0x0095613B,
//   0x00956192, 0x0096A49F, 0x0096A5A9, 0x00971FB6). The catalog label "Position_FilterNearbyThenFind" is descriptive.
// Callees: the STLport __find_if @ 0x008AAC50 (its out-of-line predicate is unary_negate::operator() 0x008AABC0, this
//   chunk) and remove_copy_if @ 0x008AB710.
// Shape: STLport 4.x `remove_if(first, last, pred)` in a /Gr TU (`__fastcall`: first in ecx, last in edx -> esi, the
//   16-byte predicate by value on the stack): find_if inlined to `__find_if(first, last, pred, random_access_iterator_tag())`
//   with the empty tag homed in the live predicate slot (`lea eax,[esp+0Ch]` pushed first), each by-value predicate
//   copied dword by dword into a `sub esp,10h` block, then remove_copy_if(++next, last, first, pred).
//   GOTCHAS 24l: the tag lands in the live slot only with __find_if's real body visible (same-TU auto_inline(off) +
//   noinline stand-in below).
struct C3DVector {
    float X, Y, Z;
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
};

struct C2DVector {
    float X, Y;
    C2DVector(float x, float y) : X(x), Y(y) { }
    float GetLengthSquared() const { return X * X + Y * Y; }
};

class CVectorFilter_FurtherFromPointThan {      // Ego_r.pdb, 16 bytes
public:
    float     Radius;                           // +0x00
    C3DVector CentrePos;                        // +0x04
    CVectorFilter_FurtherFromPointThan(const CVectorFilter_FurtherFromPointThan& other)
        : Radius(other.Radius), CentrePos(other.CentrePos) { }
    bool operator()(const C3DVector& pos) const
    {
        C2DVector delta(pos.X - CentrePos.X, pos.Y - CentrePos.Y);
        float radius = Radius;
        return delta.GetLengthSquared() > radius * radius;
    }
};

namespace std {
template <class _Predicate>
class unary_negate {
protected:
    _Predicate _M_pred;
public:
    bool operator()(const C3DVector& __x) const { return !_M_pred(__x); }   // out of line @ 0x008AABC0
};
}

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
template C3DVector* __fastcall remove_if<C3DVector*, unary_negate<CVectorFilter_FurtherFromPointThan> >(
    C3DVector*, C3DVector*, unary_negate<CVectorFilter_FurtherFromPointThan>);
}