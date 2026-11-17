// std::_Rb_tree<void*, pair<void* const, float>, _Select1st<pair<void* const, float> >, less<void* >, allocator<pair<void* const, float> > >::_M_insert(x, y, v, w) -- STLport 4.6 _tree.c: create the node (_M_create_node 0x00580428), link it as y's left or right child (fixing root / leftmost / rightmost), rebalance (_Rb_global<bool>::_Rebalance 0x0042971D), count it, return the iterator
// Leaf: ?_M_insert@?$_Rb_tree@PAXU?$pair@QAXM@std@@U?$_Select1st@U?$pair@QAXM@std@@@2@U?$less@PAX@2@V?$allocator@U?$pair@QAXM@std@@@2@@std@@AAE?AU?$_Rb_tree_iterator@U?$pair@QAXM@std@@U?$_Nonconst_traits@U?$pair@QAXM@std@@@2@@2@PAU_Rb_tree_node_base@2@0ABU?$pair@QAXM@2@0@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005808A0, 136 bytes, thiscall `ret 14h` (hidden iterator result + x, y, &v, w);
//   frame-pointer size TU (`optimize("y",off)` + `("s",on)`).
// Identity (NF58, round 7): the map<void*, float> tree; Ego_r.pdb members of this type: CTCHaste::Multipliers. Landed callers of its
//   insert_unique 0x00580D21: 0x005812AE CBlockTable_005812ae::GetBlockData; 0x00694020 CCreatureActionBase::FrameUpdate; 0x006A02B0
//   CCreatureAction_UnsheatheItemFromInventoryWithHasteAction::FinishActio; 0x0078B410
//   CCreatureAction_HeroLoadRangedWeapon::Initialise; 0x008C7280 CCreatureAction_TrollGroundPound::Initialise; 0x008CDD40
//   CCreatureAction_ReleaseTimeSpell::FinishAction; 0x008E6780 CCreatureAction_CastStrengthMultiStrike::Initialise.
// Retail facts: unsigned (`jb`) key compare; _M_create_node 0x00580428 allocates 0x18-byte nodes (`push 18h`) = 0x10 links + the 8-byte
//   value_type.
//   STLport 4.6.2 _tree.c (private member), descriptive-by-library (both PDBs are Dinkumware builds). Was the stand-in
//   `CRetailTree_ulongkey_buy00580428::Insert`; now the explicit instantiation of stlport/fable_map.h
//   FABLE_RETAIL_STLP_MAP_ORIGINAL_INSERT_BODY (the landed 0x0042CB08 pattern).
#pragma optimize("y", off)
#pragma optimize("s", on)
#define FABLE_RETAIL_STLP_MAP_ORIGINAL_INSERT_BODY
#include "stlport/fable_map.h"

namespace std {
template _Rb_tree<void*, pair<void* const, float>, _Select1st<pair<void* const, float> >, less<void* >, allocator<pair<void* const, float> > >::iterator _Rb_tree<void*, pair<void* const, float>, _Select1st<pair<void* const, float> >, less<void* >, allocator<pair<void* const, float> > >::_M_insert(_Rb_tree_node_base*, _Rb_tree_node_base*, const pair<void* const, float>&, _Rb_tree_node_base*);
}
