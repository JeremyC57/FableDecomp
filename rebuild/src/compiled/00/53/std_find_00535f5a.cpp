// std::__find(const unsigned long* first, const unsigned long* last, const unsigned long& val, const random_access_iterator_tag&) -- (const unsigned long*, unsigned long) STLport linear search unrolled by four: the first element equal to val, else last
// Leaf: ??$__find@PBKK@std@@YIPBKPBK0ABKABUrandom_access_iterator_tag@0@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00535F5A, 100 bytes, `__fastcall` (first in ecx, last in edx; &val and the unused
//   tag reference on the stack, `ret 8`); size-optimised COMDAT (`push 4 ; pop ebx`, no 16-byte alignment).
// Identity (proven): std::__find<const unsigned long*, unsigned long>; evidence: NUISystem::CList::GetDataFromDef 0x0053822B (through its find forwarder 0x0053C8F8) and CDynamicList::GetDataFromDef 0x00541390 (directly) search the const definition's index vector for the child index -- Ego_r.pdb CUIDef::NonScrollingChildren, vector<unsigned long>, read through CDefPointer<const CUIDef>; FableWin.pdb's matching Dinkumware instantiation is _Find<const unsigned long*, unsigned long> (??$_Find@PBKK@std@@).
// Label: the manifest's "std::__find_in_int_array_16" is propagated / behavioural (family J096: five byte-identical 4-byte-element
//   searches; retail has no identical-code folding, so each address is its own instantiation).
// Callees: none.
// Shape: STLport 4.6 _algobase.c `__find(first, last, val, const random_access_iterator_tag&)`: trip count (last - first) >> 2,
//   four compares per trip, then `switch (last - first)` falling through 3 / 2 / 1.
#pragma optimize("s", on)

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
template const unsigned long* __fastcall __find<const unsigned long*, unsigned long >(const unsigned long*, const unsigned long*, unsigned long const&, const random_access_iterator_tag&);
} // namespace std