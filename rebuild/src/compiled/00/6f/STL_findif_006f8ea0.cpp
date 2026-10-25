// _STL::__find_if(const NAnimationEvents::CEvent* first, const NAnimationEvents::CEvent* last, CIsEventIndexEqualTo pred, const random_access_iterator_tag&) -- STLport four-way unrolled find_if: the first animation event whose 30-bit index equals pred.Index, else last
// Leaf: ??$__find_if@PBVCEvent@NAnimationEvents@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006F8EA0, 152 bytes, `__fastcall` (first in ecx, last in edx, the 4-byte
//   predicate by value and the tag reference on the stack, `ret 8`), returns the iterator in eax.
// Identity: the manifest's "std::vector::Find_ByMask" is descriptive. The callers are the landed
//   NAnimationEvents::CManager::AnimHasExtraEvent 0x006F9640 (whose same-TU stand-in is this body:
//   `find_if(events->begin(), events->end(), CIsEventIndexEqualTo(index))`), GetExtraEventTime 0x006F96C0 and
//   0x006FB110; Ego_r.pdb CEvent = { unsigned long Index : 30; ExternallyImported : 1; StopEvent : 1; float Time; }
//   (8 bytes, hence the `and 3FFFFFFFh`) and CIsEventIndexEqualTo = { unsigned long Index; } (4 bytes).
// Callees: none (the predicate is inline).
// Shape: STLport 4.6 _algo.h `__find_if(.., const random_access_iterator_tag&)` verbatim: `__trip_count = (last -
//   first) >> 2` (`sar ecx,5` on the byte distance), four inline predicate tests per trip with pred.Index cached in esi,
//   then the `switch (last - first)` fall-through over the 3/2/1 remainder, `return last` for case 0/default.
namespace NAnimationEvents {
class CEvent {                                              // Ego_r.pdb: 8 bytes
public:
    unsigned long Index : 30;                               // +0x00
    unsigned long ExternallyImported : 1;
    unsigned long StopEvent : 1;
    float         Time;                                     // +0x04
};
class CIsEventIndexEqualTo {                                // Ego_r.pdb: 4 bytes
public:
    unsigned long Index;                                    // +0x00
    CIsEventIndexEqualTo(unsigned long index) : Index(index) { }
    bool operator()(const CEvent& event) const { return event.Index == Index; }
};
}

namespace _STL {

struct input_iterator_tag { input_iterator_tag() { } };
struct forward_iterator_tag : public input_iterator_tag { forward_iterator_tag() { } };
struct bidirectional_iterator_tag : public forward_iterator_tag { bidirectional_iterator_tag() { } };
struct random_access_iterator_tag : public bidirectional_iterator_tag { random_access_iterator_tag() { } };

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

template const NAnimationEvents::CEvent* __fastcall __find_if(const NAnimationEvents::CEvent*,
                                                              const NAnimationEvents::CEvent*,
                                                              NAnimationEvents::CIsEventIndexEqualTo,
                                                              const random_access_iterator_tag&);

} // namespace _STL