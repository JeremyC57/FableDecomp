// std::_Rb_tree<EHeroAbility, pair<EHeroAbility const, SExpSpendUndo>, _Select1st<pair<EHeroAbility const, SExpSpendUndo> >, less<EHeroAbility >, allocator<pair<EHeroAbility const, SExpSpendUndo> > >::insert_unique(const value_type& v) -- STLport 4.6 _tree.c unique insert: descend on key < node key, step back with _Rb_global<bool>::_M_decrement (0x004294E2) unless at begin(), then _M_insert (0x0057A060) when the predecessor's key is less; pair<iterator, bool> through the hidden pointer
// Leaf: ?insert_unique@?$_Rb_tree@W4EHeroAbility@@U?$pair@$$CBW4EHeroAbility@@USExpSpendUndo@@@std@@U?$_Select1st@U?$pair@$$CBW4EHeroAbility@@USExpSpendUndo@@@std@@@3@U?$less@W4EHeroAbility@@@3@V?$allocator@U?$pair@$$CBW4EHeroAbility@@USExpSpendUndo@@@std@@@3@@std@@QAE?AU?$pair@U?$_Rb_tree_iterator@U?$pair@$$CBW4EHeroAbility@@USExpSpendUndo@@@std@@U?$_Nonconst_traits@U?$pair@$$CBW4EHeroAbility@@USExpSpendUndo@@@std@@@2@@std@@_N@2@ABU?$pair@$$CBW4EHeroAbility@@USExpSpendUndo@@@2@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0057A1AA, 134 bytes, thiscall `ret 8`; frame-pointer size TU.
// Identity (NF58, round 7): the map<EHeroAbility, SExpSpendUndo> tree; Ego_r.pdb members of this type:
//   CTCInventoryExperience::UndoableAbilityBuys. Landed callers of its insert_unique 0x0057A1AA: 0x005785B3
//   CTCInventoryExperience::BuyAbility.
//   Its callee _M_insert 0x0057A060 is the same instance. STLport 4.6.2 source, descriptive-by-library. Was the stand-in
//   `CRetailTree_longkey_ins0057a060::insert`; now an explicit instantiation over stlport/fable_map.h (FABLE_RETAIL_STLP_MAP body;
//   FABLE_RETAIL_PASS22_LIP_SYNC_SAMPLE_READ: out-of-line _M_decrement), the landed 0x0042CEE3 pattern.
#pragma optimize("y", off)
#pragma optimize("s", on)
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_PASS22_LIP_SYNC_SAMPLE_READ
#include "stlport/fable_map.h"
#define FABLE_RETAIL_SEXPSPENDUNDO_ONLY
#include "engine/EHeroAbility.h"
#include "engine/CTCInventoryExperience.h"
namespace std {
template pair<_Rb_tree<EHeroAbility, pair<EHeroAbility const, SExpSpendUndo>, _Select1st<pair<EHeroAbility const, SExpSpendUndo> >, less<EHeroAbility >, allocator<pair<EHeroAbility const, SExpSpendUndo> > >::iterator, bool> _Rb_tree<EHeroAbility, pair<EHeroAbility const, SExpSpendUndo>, _Select1st<pair<EHeroAbility const, SExpSpendUndo> >, less<EHeroAbility >, allocator<pair<EHeroAbility const, SExpSpendUndo> > >::insert_unique(const pair<EHeroAbility const, SExpSpendUndo>&);
}
