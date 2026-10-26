// std::__find(EHeroAbility* first, EHeroAbility* last, const EHeroAbility& val, const random_access_iterator_tag&) -- (EHeroAbility*, EHeroAbility) STLport linear search unrolled by four: the first element equal to val, else last
// Leaf: ??$__find@PAW4EHeroAbility@@W41@@std@@YIPAW4EHeroAbility@@PAW41@0ABW41@ABUrandom_access_iterator_tag@0@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005F7ACC, 100 bytes, `__fastcall` (first in ecx, last in edx; &val and the unused
//   tag reference on the stack, `ret 8`); size-optimised COMDAT (`push 4 ; pop ebx`, no 16-byte alignment).
// Identity (proven): std::__find<EHeroAbility*, EHeroAbility>; evidence: CTCInventoryAbilities::SetAbilityAsAvailable 0x005F6BD6 (Ego_r.pdb ?SetAbilityAsAvailable@CTCInventoryAbilities@@QAEXW4EHeroAbility@@_N1@Z) searches this+0x154 for its EHeroAbility argument through the forwarder 0x005F8150, as do two more CTCInventoryAbilities sites on this+0x17C -- the class's vector<EHeroAbility> members (ReceivedAbilityOrder / AssignedAbilities); FableWin.pdb holds _Find<EHeroAbility*, EHeroAbility> (??$_Find@PAW4EHeroAbility@@W41@@std@@).
// Label: the manifest's "std_find_Linear4Byte" is propagated / behavioural (family J096: five byte-identical 4-byte-element
//   searches; retail has no identical-code folding, so each address is its own instantiation).
// Callees: none.
// Shape: STLport 4.6 _algobase.c `__find(first, last, val, const random_access_iterator_tag&)`: trip count (last - first) >> 2,
//   four compares per trip, then `switch (last - first)` falling through 3 / 2 / 1.
#pragma optimize("s", on)
enum EHeroAbility { };                           // Ego_r.pdb (enumerators not needed)
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
template EHeroAbility* __fastcall __find<EHeroAbility*, EHeroAbility >(EHeroAbility*, EHeroAbility*, EHeroAbility const&, const random_access_iterator_tag&);
} // namespace std