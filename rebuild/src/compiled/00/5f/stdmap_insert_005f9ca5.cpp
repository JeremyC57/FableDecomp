// std::map<EHeroAbility, NUISystem::CComponent*, std::less<EHeroAbility>, std::allocator<std::pair<EHeroAbility const, NUISystem::CComponent*> > >::insert(std::_Rb_tree_iterator<std::pair<EHeroAbility const, NUISystem::CComponent*> >, std::pair<EHeroAbility const, NUISystem::CComponent*> const&) -- STLport 4.6 insert (stlport/fable_map.h)
// Leaf: ?insert@?$map@W4EHeroAbility@@PAVCComponent@NUISystem@@U?$less@W4EHeroAbility@@@std@@V?$allocator@U?$pair@$$CBW4EHeroAbility@@PAVCComponent@NUISystem@@@std@@@5@@std@@QAE?AU?$_Rb_tree_iterator@U?$pair@$$CBW4EHeroAbility@@PAVCComponent@NUISystem@@@std@@U?$_Nonconst_traits@U?$pair@$$CBW4EHeroAbility@@PAVCComponent@NUISystem@@@std@@@2@@2@U32@ABU?$pair@$$CBW4EHeroAbility@@PAVCComponent@NUISystem@@@2@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005F9CA5, oracle length 29 bytes.
// Identity: (1) the landed caller 0x005F9EA5 (std::map<EHeroAbility, NUISystem::CComponent *, std::less<EHeroAbility>, std::allocator<std::pair<EHeroAbility const, NUISystem::CComponent *>>>::operator[]) binds this map's insert (its iterator spelled without the traits argument: a binder spelling to fix, see NOTES) (rebuild/oracles/reloc_bindings.tsv); (2) Ego_r.pdb / FableWin.pdb carry the map<W4EHeroAbility@@PAVCComponent@NUISystem@@> instantiation (_Tmap_traits@W4EHeroAbility@@PAVCComponent@NUISystem@@U?$less@).
// Model: the shared stlport/fable_map.h (STLport 4.6 _tree.h / _tree.c; FABLE_RETAIL_STLP_MAP); the explicit instantiation emits the member as
//   its own COMDAT. Replaces the generated stand-in (was _unattributed, a descriptive placeholder class).
#pragma optimize("s",on)
#define FABLE_RETAIL_STLP_MAP
#include "stlport/fable_map.h"
#include "engine/EHeroAbility.h"
namespace NUISystem { class CComponent; }
typedef std::map<EHeroAbility, NUISystem::CComponent*> _Map_005f9ca5;
template _Map_005f9ca5::iterator _Map_005f9ca5::insert(_Map_005f9ca5::iterator, const _Map_005f9ca5::value_type&);
