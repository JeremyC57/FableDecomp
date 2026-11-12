// std::find<EHeroAbility*, EHeroAbility>(EHeroAbility* first, EHeroAbility* last, const EHeroAbility& val) -- STLport find: forward to __find with a random_access_iterator_tag temporary
// Leaf: ??$find@PAW4EHeroAbility@@W41@@std@@YIPAW4EHeroAbility@@PAW41@0ABW41@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005F8150, `__fastcall` (first in ecx, last in edx, &val on the stack), size-optimised COMDAT.
// Naming (agentNF31, 2026-11-12): was the stand-in vector<EWeaponClass>::_Umove (AllocT::construct_005f7acc). DESCRIPTIVE-BY-LIBRARY
//   (retail STLport; FableWin.pdb, a Dinkumware build, holds the equivalent ??$_Find@PAW4EHeroAbility@@W41@@std@@YIPAW4EHeroAbility@@PAW41@0ABW41@@Z).
//   Its only callee is the landed std::__find<EHeroAbility*, EHeroAbility> 0x005F7ACC (bound under that row's own leaf), and the
//   landed caller CTCInventoryAbilities::SetAbilityAsAvailable 0x005F6BD6 binds this symbol, calling it on ReceivedAbilityOrder
//   begin/end/&ability. STLport 4.6 _algobase.h find(first, last, val) = __find(first, last, val, _STLP_ITERATOR_CATEGORY(first, ..)).
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
                                    const random_access_iterator_tag&);   // 0x005F7ACC (landed row, out of line)
template <class _InputIter, class _Tp>
_InputIter __fastcall find(_InputIter __first, _InputIter __last, const _Tp& __val)
{
    return __find(__first, __last, __val, random_access_iterator_tag());
}
}
template EHeroAbility* __fastcall std::find<EHeroAbility*, EHeroAbility>(EHeroAbility*, EHeroAbility*, const EHeroAbility&);
