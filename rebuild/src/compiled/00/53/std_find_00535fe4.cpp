// std::__find(NUISystem::CComponent** first, NUISystem::CComponent** last, const NUISystem::CComponent*& val, const random_access_iterator_tag&) -- (NUISystem::CComponent**, NUISystem::CComponent*) STLport linear search unrolled by four: the first element equal to val, else last
// Leaf: ??$__find@PAPAVCComponent@NUISystem@@PAV12@@std@@YIPAPAVCComponent@NUISystem@@PAPAV12@0ABQAV12@ABUrandom_access_iterator_tag@0@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00535FE4, 100 bytes, `__fastcall` (first in ecx, last in edx; &val and the unused
//   tag reference on the stack, `ret 8`); size-optimised COMDAT (`push 4 ; pop ebx`, no 16-byte alignment).
// Identity (proven): std::__find<NUISystem::CComponent**, NUISystem::CComponent*>; evidence: NUISystem::CList::RemoveChild 0x0053B321 (through its forwarder 0x0053C91C), CDynamicList::DeleteChild 0x00541120 and CScrollingViewport::RemoveChild 0x0054E980 (landed) search their vector<CComponent*> child lists (Ego_r.pdb CList::ScrollingChildren / NonScrollingChildren) for the component pointer; FableWin.pdb holds _Find<CComponent**, CComponent*> (??$_Find@PAPAVCComponent@NUISystem@@PAV12@@std@@).
// Label: the manifest's "std_find<int>" is propagated / behavioural (family J096: five byte-identical 4-byte-element
//   searches; retail has no identical-code folding, so each address is its own instantiation).
// Callees: none.
// Shape: STLport 4.6 _algobase.c `__find(first, last, val, const random_access_iterator_tag&)`: trip count (last - first) >> 2,
//   four compares per trip, then `switch (last - first)` falling through 3 / 2 / 1.
#pragma optimize("s", on)
namespace NUISystem { class CComponent; }
namespace std {
struct input_iterator_tag { };
struct output_iterator_tag { };
struct forward_iterator_tag : public input_iterator_tag { };
struct bidirectional_iterator_tag : public forward_iterator_tag { };
struct random_access_iterator_tag : public bidirectional_iterator_tag { };
template <class _RandomAccessIter, class _Tp>
_RandomAccessIter __fastcall __find(_RandomAccessIter __first, _RandomAccessIter __last, const _Tp& __val,
                                    const random_access_iterator_tag&)
{
    int __trip_count = (__last - __first) >> 2;

    for (; __trip_count > 0; --__trip_count) {
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
template NUISystem::CComponent** __fastcall __find<NUISystem::CComponent**, NUISystem::CComponent* >(NUISystem::CComponent**, NUISystem::CComponent**, NUISystem::CComponent* const&, const random_access_iterator_tag&);
} // namespace std