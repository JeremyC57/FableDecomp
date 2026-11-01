// std::__find(CCountedPointer<CComponent>* first, CCountedPointer<CComponent>* last, const CCountedPointer<CComponent>& val, const random_access_iterator_tag&) -- STLport random-access linear search: four-way unrolled trip loop, then the 3/2/1 remainder switch; returns the first element equal to val (same Data pointer) or last
// Leaf: ??$__find@PAV?$CCountedPointer@VCComponent@NUISystem@@@@V1@@std@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00534EB0, 112 bytes, __fastcall (ecx = first, edx = last), `ret 8` (val and the unused
//   tag by reference on the stack).
// Identity (proven): the callers NUISystem::CComponent::RemoveChild 0x00533BC0 (landed; it binds this address as
//   ??$__find@PAV?$CCountedPointer@VCComponent@NUISystem@@@@V1@@std@@YI..., the find(Children.begin(), Children.end(),
//   Children[index])), 0x00532C55 (CGuiComponent update, same vector at +0xbc/+0xc0) and 0x0053530A pass first/last of an STLport
//   vector<CCountedPointer<NUISystem::CComponent> > (8-byte elements: `sar ecx,5` = (bytes/8)>>2) and a reference to one element;
//   the compare reads only the Data word -- FableWin.pdb's free __fastcall
//   ??$?8VCComponent@NUISystem@@V01@@@YI_NABV?$CCountedPointer@VCComponent@NUISystem@@@@0@Z (operator==, inlined). The body is
//   STLport 4.6 _algobase.c `__find` for random-access iterators verbatim. The catalog label "std::__find_in_uint_array" is a
//   descriptive guess (the uint-array instantiation is __find<unsigned long*, unsigned long> 0x005348E0).
// Shape: an explicit instantiation of the /Gr algorithm-TU template (`__fastcall` on the definition).
#include "engine/CCountedPointer.h"
namespace NUISystem { class CComponent; }
template <class T, class U>
inline bool __fastcall operator==(const CCountedPointer<T>& lhs, const CCountedPointer<U>& rhs)   // FableWin ??$?8
{
    return lhs.Get() == rhs.Get();
}
namespace std {
struct random_access_iterator_tag { random_access_iterator_tag() { } };
template <class _RandomAccessIter, class _Tp>
_RandomAccessIter __fastcall __find(_RandomAccessIter __first, _RandomAccessIter __last,
                                    const _Tp& __val, const random_access_iterator_tag&)
{
    int __trip_count = (__last - __first) >> 2;
    for ( ; __trip_count > 0 ; --__trip_count) {
        if (*__first == __val) return __first;
        ++__first;
        if (*__first == __val) return __first;
        ++__first;
        if (*__first == __val) return __first;
        ++__first;
        if (*__first == __val) return __first;
        ++__first;
    }
    switch (__last - __first) {
    case 3:
        if (*__first == __val) return __first;
        ++__first;
    case 2:
        if (*__first == __val) return __first;
        ++__first;
    case 1:
        if (*__first == __val) return __first;
        ++__first;
    case 0:
    default:
        return __last;
    }
}
template CCountedPointer<NUISystem::CComponent>* __fastcall
__find<CCountedPointer<NUISystem::CComponent>*, CCountedPointer<NUISystem::CComponent> >(
    CCountedPointer<NUISystem::CComponent>*, CCountedPointer<NUISystem::CComponent>*,
    const CCountedPointer<NUISystem::CComponent>&, const random_access_iterator_tag&);
} // namespace std