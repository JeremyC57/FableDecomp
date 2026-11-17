// std::_Rb_tree<unsigned __int64, pair<unsigned __int64 const, CThingCreatureBase*>, _Select1st<pair<unsigned __int64 const, CThingCreatureBase*> >, less<unsigned __int64 >, allocator<pair<unsigned __int64 const, CThingCreatureBase*> > >::_M_insert(x, y, v, w) -- STLport 4.6 _tree.c: allocate and copy-construct the node (_M_create_node, inline here), link it as y's left or right child (fixing root / leftmost / rightmost), rebalance (_Rb_global<bool>::_Rebalance 0x0042971D), count it, return the iterator
// Leaf: ?_M_insert@?$_Rb_tree@_KU?$pair@$$CB_KPAVCThingCreatureBase@@@std@@U?$_Select1st@U?$pair@$$CB_KPAVCThingCreatureBase@@@std@@@2@U?$less@_K@2@V?$allocator@U?$pair@$$CB_KPAVCThingCreatureBase@@@std@@@2@@std@@AAE?AU?$_Rb_tree_iterator@U?$pair@$$CB_KPAVCThingCreatureBase@@@std@@U?$_Nonconst_traits@U?$pair@$$CB_KPAVCThingCreatureBase@@@std@@@2@@2@PAU_Rb_tree_node_base@2@0ABU?$pair@$$CB_KPAVCThingCreatureBase@@@2@0@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006F1280, 215 bytes, thiscall `ret 14h` (hidden iterator result + x, y, &v, w); /O2 TU.
// Identity (NF58, round 7): the map<unsigned __int64, CThingCreatureBase*> tree; Ego_r.pdb members of this type:
//   CDefendingCombatantInfo::Attackers. Landed callers of this address: 0x006F1EF0 std::_Rb_tree<unsigned __int64, std::pair<unsigned
//   __int64 const, CThingCreature.
//   STLport 4.6.2 _tree.c (private member), descriptive-by-library (both PDBs are Dinkumware builds). Was the stand-in
//   `CThingCompareMap::Insert`; now the explicit instantiation of stlport/fable_map.h
//   FABLE_RETAIL_STLP_MAP_ORIGINAL_INSERT_BODY (NF56's 0x0069D520 pattern; malloc 0x00BFEA0E is the STLport node allocator).
#define FABLE_RETAIL_STLP_MAP_ORIGINAL_INSERT_BODY
#include "stlport/fable_map.h"
class CThingCreatureBase;
namespace std {
template _Rb_tree<unsigned __int64, pair<unsigned __int64 const, CThingCreatureBase*>, _Select1st<pair<unsigned __int64 const, CThingCreatureBase*> >, less<unsigned __int64 >, allocator<pair<unsigned __int64 const, CThingCreatureBase*> > >::iterator _Rb_tree<unsigned __int64, pair<unsigned __int64 const, CThingCreatureBase*>, _Select1st<pair<unsigned __int64 const, CThingCreatureBase*> >, less<unsigned __int64 >, allocator<pair<unsigned __int64 const, CThingCreatureBase*> > >::_M_insert(_Rb_tree_node_base*, _Rb_tree_node_base*, const pair<unsigned __int64 const, CThingCreatureBase*>&, _Rb_tree_node_base*);
}
