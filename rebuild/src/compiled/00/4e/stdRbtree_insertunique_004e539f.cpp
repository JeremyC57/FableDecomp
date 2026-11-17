// std::_Rb_tree<long, pair<long const, long>, _Select1st<pair<long const, long> >, less<long >, allocator<pair<long const, long> > >::insert_unique(const value_type& v) -- STLport 4.6 _tree.c unique insert: descend on key < node key, step back with _Rb_global<bool>::_M_decrement (0x004294E2) unless at begin(), then _M_insert (0x004E5425) when the predecessor's key is less; pair<iterator, bool> through the hidden pointer
// Leaf: ?insert_unique@?$_Rb_tree@JU?$pair@$$CBJJ@std@@U?$_Select1st@U?$pair@$$CBJJ@std@@@2@U?$less@J@2@V?$allocator@U?$pair@$$CBJJ@std@@@2@@std@@QAE?AU?$pair@U?$_Rb_tree_iterator@U?$pair@$$CBJJ@std@@U?$_Nonconst_traits@U?$pair@$$CBJJ@std@@@2@@std@@_N@2@ABU?$pair@$$CBJJ@2@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004E539F, 134 bytes, thiscall `ret 8`; frame-pointer size TU.
// Identity (NF58, round 7): the map<long, long> tree; Ego_r.pdb members of this type:
//   CExperienceDef::PickupParticleEffectsPerExperiencePoints, CGameScriptInterface::ScriptTimers,
//   CGameScriptInterface.conflict::ScriptTimers, CTCInventoryTrade::SpecialItemsToDelete (+1 more). Landed callers of its insert_unique
//   0x004E539F: 0x005B2A37 std::_Rb_tree<long, std::pair<long const, long>, std::_Select1st<std::. Landed callers of _M_insert besides
//   0x004E539F: 0x005B2A37 std::_Rb_tree<long, std::pair<long const, long>, std::_Select1st<std::.
//   Its callee _M_insert 0x004E5425 is the same instance. STLport 4.6.2 source, descriptive-by-library. Was the stand-in
//   `std::_Rb_tree<...>::insert_unique`; now an explicit instantiation over stlport/fable_map.h (FABLE_RETAIL_STLP_MAP body;
//   FABLE_RETAIL_PASS22_LIP_SYNC_SAMPLE_READ: out-of-line _M_decrement), the landed 0x0042CEE3 pattern.
#pragma optimize("y", off)
#pragma optimize("s", on)
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_PASS22_LIP_SYNC_SAMPLE_READ
#include "stlport/fable_map.h"

namespace std {
template pair<_Rb_tree<long, pair<long const, long>, _Select1st<pair<long const, long> >, less<long >, allocator<pair<long const, long> > >::iterator, bool> _Rb_tree<long, pair<long const, long>, _Select1st<pair<long const, long> >, less<long >, allocator<pair<long const, long> > >::insert_unique(const pair<long const, long>&);
}
