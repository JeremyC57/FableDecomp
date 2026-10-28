// std::__find_if(NAnimationEvents::CEvent* first, NAnimationEvents::CEvent* last, CFilter_Bind2<NAnimationEvents::CIsEventIndexEqualTo, NAnimationEvents::CIsEventTimeEqualTo> pred, const random_access_iterator_tag&) -- STLport random-access find_if: the first animation event whose index and time both match, or last
// Leaf: ??$__find_if@PAVCEvent@NAnimationEvents@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006FB240, 340 bytes, /Gr `__fastcall` (first in ecx, last in edx), the
//   8-byte predicate by value and the tag reference on the stack, `ret 0Ch`. Callers 0x006F952A, 0x006FB62F.
// Name: Ego_r.pdb `std::_Find_if<NAnimationEvents::CEvent*, CFilter_Bind2<CIsEventIndexEqualTo, CIsEventTimeEqualTo> >`
//   is the Dinkumware counterpart; retail is STLport's `__find_if` (twin shape: landed 0x008060C0).
// Layout: Ego_r.pdb NAnimationEvents::CEvent (8: Index:30, ExternallyImported:1, StopEvent:1, float Time @+4),
//   CIsEventIndexEqualTo (ulong Index), CIsEventTimeEqualTo (float Time), CFilter_Bind2<..> (8: F1, F2 references).
// Shape: index test on the 30-bit field, then fabs(event.Time - Time) < 0.0001f (constant 0x0129BA3C); the key load is
//   hoisted out of the unrolled loop by the optimiser and reloaded in the 3/2/1 remainder.
#include <math.h>

namespace NAnimationEvents {
class CEvent {                                 // 8 bytes
public:
    unsigned long Index : 30;                  // +0x0
    unsigned long ExternallyImported : 1;
    unsigned long StopEvent : 1;
    float Time;                                // +0x4
};

class CIsEventIndexEqualTo {
public:
    unsigned long Index;
    bool operator()(const CEvent& event) const { return event.Index == Index; }
};

class CIsEventTimeEqualTo {
public:
    float Time;
    bool operator()(const CEvent& event) const { return fabs(event.Time - Time) < 0.0001f; }
};
}

template <class F1T, class F2T>
class CFilter_Bind2 {
public:
    const F1T& F1;
    const F2T& F2;
    template <class T> bool operator()(const T& t) const { return F1(t) && F2(t); }
};

namespace std {
struct random_access_iterator_tag { random_access_iterator_tag() { } };

template <class _RandomAccessIter, class _Predicate>
_RandomAccessIter __fastcall __find_if(_RandomAccessIter __first, _RandomAccessIter __last, _Predicate __pred,
                                       const random_access_iterator_tag&)
{
    int __trip_count = (__last - __first) >> 2;
    for (; __trip_count > 0; --__trip_count) {
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
    case 0:
    default:
        return __last;
    }
}

template NAnimationEvents::CEvent* __fastcall
__find_if(NAnimationEvents::CEvent*, NAnimationEvents::CEvent*,
          CFilter_Bind2<NAnimationEvents::CIsEventIndexEqualTo, NAnimationEvents::CIsEventTimeEqualTo>,
          const random_access_iterator_tag&);
}