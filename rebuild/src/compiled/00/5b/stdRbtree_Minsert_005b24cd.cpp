// std::_Rb_tree<NUISystem::CComponent*, pair<NUISystem::CComponent* const, std::pair<CWideString, long> >, _Select1st<pair<NUISystem::CComponent* const, std::pair<CWideString, long> > >, less<NUISystem::CComponent* >, allocator<pair<NUISystem::CComponent* const, std::pair<CWideString, long> > > >::_M_insert(x, y, v, w) -- STLport 4.6 _tree.c: create the node (_M_create_node 0x005B204B), link it as y's left or right child (fixing root / leftmost / rightmost), rebalance (_Rb_global<bool>::_Rebalance 0x0042971D), count it, return the iterator
// Leaf: ?_M_insert@?$_Rb_tree@PAVCComponent@NUISystem@@U?$pair@QAVCComponent@NUISystem@@U?$pair@VCWideString@@J@std@@@std@@U?$_Select1st@U?$pair@QAVCComponent@NUISystem@@U?$pair@VCWideString@@J@std@@@std@@@4@U?$less@PAVCComponent@NUISystem@@@4@V?$allocator@U?$pair@QAVCComponent@NUISystem@@U?$pair@VCWideString@@J@std@@@std@@@4@@std@@AAE?AU?$_Rb_tree_iterator@U?$pair@QAVCComponent@NUISystem@@U?$pair@VCWideString@@J@std@@@std@@U?$_Nonconst_traits@U?$pair@QAVCComponent@NUISystem@@U?$pair@VCWideString@@J@std@@@std@@@2@@2@PAU_Rb_tree_node_base@2@0ABU?$pair@QAVCComponent@NUISystem@@U?$pair@VCWideString@@J@std@@@2@0@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005B24CD, 136 bytes, thiscall `ret 14h` (hidden iterator result + x, y, &v, w);
//   frame-pointer size TU (`optimize("s", on)`).
// Identity (NF58, round 7): the map<NUISystem::CComponent*, std::pair<CWideString, long>> tree; Ego_r.pdb members of this type:
//   CTCInventoryTrade::NumBuyableThings, CTCInventoryTrade::NumSellableThings, CTCInventoryTrade::NumWantedThings. Landed callers of
//   its insert_unique 0x005B2555: 0x005B2B76 std::_Rb_tree<NUISystem::CComponent *, std::pair<NUISystem::CComponent. Landed callers of
//   _M_insert besides 0x005B2555: 0x005B2B76 std::_Rb_tree<NUISystem::CComponent *, std::pair<NUISystem::CComponent.
// Retail facts: unsigned (`jb`) key compare; _M_create_node 0x005B204B allocates 0x1C-byte nodes (`push 1Ch`) = 0x10 links + the
//   12-byte value_type.
//   STLport 4.6.2 _tree.c (private member), descriptive-by-library (both PDBs are Dinkumware builds). Was the stand-in
//   `CRetailTree_ulongkey_buy005b204b::Insert`; now the explicit instantiation of stlport/fable_map.h
//   FABLE_RETAIL_STLP_MAP_ORIGINAL_INSERT_BODY (the landed 0x0042CB08 pattern).
// NF419 (2026-11-25): no `#pragma optimize("y", off)`: the leaf is byte- and relocation-identical without it (size mode keeps
//   its frame); it only framed end-of-TU template and compiler-generated copies (each copy checked in the T3 nf416 audit).
#pragma optimize("s", on)
#define FABLE_RETAIL_STLP_MAP_ORIGINAL_INSERT_BODY
#include "stlport/fable_map.h"
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#include "fable_string.h"
namespace NUISystem { class CComponent; }
namespace std {
template _Rb_tree<NUISystem::CComponent*, pair<NUISystem::CComponent* const, std::pair<CWideString, long> >, _Select1st<pair<NUISystem::CComponent* const, std::pair<CWideString, long> > >, less<NUISystem::CComponent* >, allocator<pair<NUISystem::CComponent* const, std::pair<CWideString, long> > > >::iterator _Rb_tree<NUISystem::CComponent*, pair<NUISystem::CComponent* const, std::pair<CWideString, long> >, _Select1st<pair<NUISystem::CComponent* const, std::pair<CWideString, long> > >, less<NUISystem::CComponent* >, allocator<pair<NUISystem::CComponent* const, std::pair<CWideString, long> > > >::_M_insert(_Rb_tree_node_base*, _Rb_tree_node_base*, const pair<NUISystem::CComponent* const, std::pair<CWideString, long> >&, _Rb_tree_node_base*);
}
