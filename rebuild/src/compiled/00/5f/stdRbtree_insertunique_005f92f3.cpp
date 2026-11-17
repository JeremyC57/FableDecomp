// std::_Rb_tree<EHeroAbility, pair<EHeroAbility const, NUISystem::CComponent*>, _Select1st<pair<EHeroAbility const, NUISystem::CComponent*> >, less<EHeroAbility >, allocator<pair<EHeroAbility const, NUISystem::CComponent*> > >::insert_unique(const value_type& v) -- STLport 4.6 _tree.c unique insert: descend on key < node key, step back with _Rb_global<bool>::_M_decrement (0x004294E2) unless at begin(), then _M_insert (0x005F926B) when the predecessor's key is less; pair<iterator, bool> through the hidden pointer
// Leaf: ?insert_unique@?$_Rb_tree@W4EHeroAbility@@U?$pair@$$CBW4EHeroAbility@@PAVCComponent@NUISystem@@@std@@U?$_Select1st@U?$pair@$$CBW4EHeroAbility@@PAVCComponent@NUISystem@@@std@@@3@U?$less@W4EHeroAbility@@@3@V?$allocator@U?$pair@$$CBW4EHeroAbility@@PAVCComponent@NUISystem@@@std@@@3@@std@@QAE?AU?$pair@U?$_Rb_tree_iterator@U?$pair@$$CBW4EHeroAbility@@PAVCComponent@NUISystem@@@std@@U?$_Nonconst_traits@U?$pair@$$CBW4EHeroAbility@@PAVCComponent@NUISystem@@@std@@@2@@std@@_N@2@ABU?$pair@$$CBW4EHeroAbility@@PAVCComponent@NUISystem@@@2@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005F92F3, 134 bytes, thiscall `ret 8`; frame-pointer size TU.
// Identity (NF58, round 7): the map<EHeroAbility, NUISystem::CComponent*> tree; Ego_r.pdb members of this type:
//   CTCInventoryAbilities::UnassignedIcons, CTCInventoryAbilities::AbilitiesMenuEntries, CTCInventoryExperience::UpgradableAbilities.
//   Landed callers of its insert_unique 0x005F92F3: 0x005F9853 std::_Rb_tree<EHeroAbility, std::pair<EHeroAbility const, NUISystem::C.
//   Landed callers of _M_insert besides 0x005F92F3: 0x005F9853 std::_Rb_tree<EHeroAbility, std::pair<EHeroAbility const, NUISystem::C.
//   Its callee _M_insert 0x005F926B is the same instance. STLport 4.6.2 source, descriptive-by-library. Was the stand-in
//   `CRetailTree_longkey_ins005f926b::insert`; now an explicit instantiation over stlport/fable_map.h (FABLE_RETAIL_STLP_MAP body;
//   FABLE_RETAIL_PASS22_LIP_SYNC_SAMPLE_READ: out-of-line _M_decrement), the landed 0x0042CEE3 pattern.
#pragma optimize("y", off)
#pragma optimize("s", on)
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_PASS22_LIP_SYNC_SAMPLE_READ
#include "stlport/fable_map.h"
#include "engine/EHeroAbility.h"
namespace NUISystem { class CComponent; }
namespace std {
template pair<_Rb_tree<EHeroAbility, pair<EHeroAbility const, NUISystem::CComponent*>, _Select1st<pair<EHeroAbility const, NUISystem::CComponent*> >, less<EHeroAbility >, allocator<pair<EHeroAbility const, NUISystem::CComponent*> > >::iterator, bool> _Rb_tree<EHeroAbility, pair<EHeroAbility const, NUISystem::CComponent*>, _Select1st<pair<EHeroAbility const, NUISystem::CComponent*> >, less<EHeroAbility >, allocator<pair<EHeroAbility const, NUISystem::CComponent*> > >::insert_unique(const pair<EHeroAbility const, NUISystem::CComponent*>&);
}
