// std::__find(const EVillageTaskType* first, const EVillageTaskType* last, const EVillageTaskType& val, const random_access_iterator_tag&) -- (const EVillageTaskType*, EVillageTaskType) STLport linear search unrolled by four: the first element equal to val, else last
// Leaf: ??$__find@PBW4EVillageTaskType@@W41@@std@@YIPBW4EVillageTaskType@@PBW41@0ABW41@ABUrandom_access_iterator_tag@0@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0065D9EE, 100 bytes, `__fastcall` (first in ecx, last in edx; &val and the unused
//   tag reference on the stack, `ret 8`); size-optimised COMDAT (`push 4 ; pop ebx`, no 16-byte alignment).
// Identity (proven): std::__find<const EVillageTaskType*, EVillageTaskType>; evidence: CTCVillage::AddThingToVillage 0x0065B854 (through the forwarder 0x0065E5A4) and CVillageDef::Validate 0x007FE670 search the village def's +0x28 vector = Ego_r.pdb CVillageDef::Tasks, vector<EVillageTaskType> (read through the const def); FableWin.pdb holds _Find<const EVillageTaskType*, EVillageTaskType> (??$_Find@PBW4EVillageTaskType@@W41@@std@@).
// Label: the manifest's "std::linear_search<EWeaponClass>" is propagated / behavioural (family J096: five byte-identical 4-byte-element
//   searches; retail has no identical-code folding, so each address is its own instantiation).
// Callees: none.
// Shape: STLport 4.6 _algobase.c `__find(first, last, val, const random_access_iterator_tag&)`: trip count (last - first) >> 2,
//   four compares per trip, then `switch (last - first)` falling through 3 / 2 / 1.
#pragma optimize("s", on)
enum EVillageTaskType { };                       // Ego_r.pdb (enumerators not needed)
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
template const EVillageTaskType* __fastcall __find<const EVillageTaskType*, EVillageTaskType >(const EVillageTaskType*, const EVillageTaskType*, EVillageTaskType const&, const random_access_iterator_tag&);
} // namespace std