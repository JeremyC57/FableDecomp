// std::__partition(C3DVector* first, C3DVector* last, CIsUsableIciclePoint pred, const bidirectional_iterator_tag&) -- move the usable icicle points to the front of [first, last), return the partition point
// Leaf: ??$__partition@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008E3430, length 137 bytes (+ int3 padding), `ret 0Ch`.
// Identity: callers 0x008DE90B and 0x008E359F (the ice-troll icicle-assault TU). Ego_r.pdb names the instance
//   `partition` / `_Partition<C3DVector*, ?A0xcee9a82c::CIsUsableIciclePoint>` (Dinkumware); the predicate is
//   { vector<CIcicleData>* Icicles; float IciclesLimit } (8 bytes). The catalog label "std_partition<C3DVector>" is
//   the right family.
// Callees: ?A0xcee9a82c::CIsUsableIciclePoint::operator()(const C3DVector&) const @ 0x008E2E40 (out of line, `this` =
//   the predicate's argument slot).
// Shape: STLport 4.x `__partition(first, last, pred, const bidirectional_iterator_tag&)` in a /Gr TU (`__fastcall`: first
//   in ecx -> esi, last in edx -> edi, the 8-byte predicate by value and the tag reference on the stack) with
//   iter_swap -> swap inlined: the 12-byte temporary goes through the frame because C3DVector has its memberwise user
//   copy constructor (as the same TU's out-of-line swap 0x008E2DB0).
class C3DVector { public:
    float X, Y, Z;
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
};

namespace NCreatureMode {
class CModeIceTrollGroundSmashIcicleAssault {
public:
    class CIcicleData;
};
}

namespace std {
template <class _Tp> class vector;
}

namespace {
class CIsUsableIciclePoint {                            // Ego_r.pdb, 8 bytes
public:
    const std::vector<NCreatureMode::CModeIceTrollGroundSmashIcicleAssault::CIcicleData>* Icicles;   // +0x00
    float IciclesLimit;                                 // +0x04
    bool operator()(const C3DVector& point) const;      // 0x008E2E40
};
}

namespace std {
struct input_iterator_tag { };
struct forward_iterator_tag : public input_iterator_tag { };
struct bidirectional_iterator_tag : public forward_iterator_tag { };

template <class _Tp>
inline void __fastcall swap(_Tp& __a, _Tp& __b)
{
    _Tp __tmp = __a;
    __a = __b;
    __b = __tmp;
}

template <class _ForwardIter1, class _ForwardIter2>
inline void iter_swap(_ForwardIter1 __a, _ForwardIter2 __b)
{
    swap(*__a, *__b);
}

template <class _BidirectionalIter, class _Predicate>
_BidirectionalIter __fastcall __partition(_BidirectionalIter __first, _BidirectionalIter __last, _Predicate __pred,
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
        iter_swap(__first, __last);
        ++__first;
    }
}
template C3DVector* __fastcall __partition<C3DVector*, CIsUsableIciclePoint>(C3DVector*, C3DVector*, CIsUsableIciclePoint,
                                                                             const bidirectional_iterator_tag&);
}