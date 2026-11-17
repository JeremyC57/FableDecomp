// CBuildingCreatureMapTree::_M_insert(_Rb_tree_node_base* x, _Rb_tree_node_base* y, const value_type& v, _Rb_tree_node_base* w) -- STLport _Rb_tree::_M_insert of map<CThingBuilding*, list<CThingAICreature*> >: hang a new node off y (left or right), fix the header links, rebalance, count it
//
// Retail 0x0051AA00, 180 bytes (+ int3 padding to 192), thiscall `ret 14h` (hidden iterator result, x, y, v, w).  Callees: 0x00BFEA0E malloc
//   (0x18-byte node), 0x00517590 pair<CThingBuilding* const, list<CThingAICreature*> > copy constructor (this chunk),
//   0x0042971D _Rb_global<bool>::_Rebalance(node, root&) (__cdecl).
// Identity: see 0x00517590 / 0x00519970 -- pointer key (`jb`), 0x18-byte nodes with a list at +0x14.
// Shape: STLport _tree.c -- `if (y == header || (w == 0 && (x != 0 || comp(KeyOfValue()(v), _S_key(y))))) { z =
//   _M_create_node(v); _S_left(y) = z; if (y == header) { _M_root() = z; _M_rightmost() = z; } else if (y ==
//   _M_leftmost()) _M_leftmost() = z; } else { z = _M_create_node(v); _S_right(y) = z; if (y == _M_rightmost())
//   _M_rightmost() = z; } _S_parent(z) = y; _S_left(z) = 0; _S_right(z) = 0; _Rebalance(z, root); ++_M_node_count;
//   return iterator(z);` with _M_create_node inline twice (malloc + placement copy with its null test on node+0x10); the
//   pair's copy constructor is only declared here, so it stays a call (as retail).
// Label: catalog "ListNode_InsertWithClone" (propagated).
// NF50 (round 7): moved onto the shared stlport/fable_map.h as the explicit instantiation of the STLport 4.6.2 _tree.c
// _M_insert body (FABLE_RETAIL_STLP_MAP_ORIGINAL_INSERT_BODY, the landed 0x0042CB08 pattern): the rebalance call binds
// ?_Rebalance@?$_Rb_global@_N@std@@SAXPAU_Rb_tree_node_base@2@AAPAU32@@Z and the row's own symbol is the STLport _M_insert its landed
// caller already binds (descriptive-by-library: both PDBs are Dinkumware builds; template arguments from the caller's binding).
// Leaf: ?_M_insert@?$_Rb_tree@PAVCThingBuilding@@U?$pair@QAVCThingBuilding@@V?$list@PAVCThingAICreature@@V?$allocator@PAVCThingAICreature@@@std@@@std@@@std@@U?$_Select1st@U?$pair@QAVCThingBuilding@@V?$list@PAVCThingAICreature@@V?$allocator@PAVCThingAICreature@@@std@@@std@@@std@@@3@U?$less@PAVCThingBuilding@@@3@V?$allocator@U?$pair@QAVCThingBuilding@@V?$list@PAVCThingAICreature@@V?$allocator@PAVCThingAICreature@@@std@@@std@@@std@@@3@@std@@AAE?AU?$_Rb_tree_iterator@U?$pair@QAVCThingBuilding@@V?$list@PAVCThingAICreature@@V?$allocator@PAVCThingAICreature@@@std@@@std@@@std@@U?$_Nonconst_traits@U?$pair@QAVCThingBuilding@@V?$list@PAVCThingAICreature@@V?$allocator@PAVCThingAICreature@@@std@@@std@@@std@@@2@@2@PAU_Rb_tree_node_base@2@0ABU?$pair@QAVCThingBuilding@@V?$list@PAVCThingAICreature@@V?$allocator@PAVCThingAICreature@@@std@@@std@@@2@0@Z
#define FABLE_RETAIL_STLP_MAP_ORIGINAL_INSERT_BODY
#include "stlport/fable_map.h"
class CThingBuilding;
class CThingAICreature;
// The pair's copy constructor is out of line in this TU (0x00517590): declared, not defined here.
template<> std::pair<CThingBuilding* const, std::list<CThingAICreature*> >::pair(const std::pair<CThingBuilding* const, std::list<CThingAICreature*> >&);
namespace std {
template _Rb_tree<CThingBuilding*, pair<CThingBuilding* const, list<CThingAICreature*> >, _Select1st<pair<CThingBuilding* const, list<CThingAICreature*> > >, less<CThingBuilding*>, allocator<pair<CThingBuilding* const, list<CThingAICreature*> > > >::iterator _Rb_tree<CThingBuilding*, pair<CThingBuilding* const, list<CThingAICreature*> >, _Select1st<pair<CThingBuilding* const, list<CThingAICreature*> > >, less<CThingBuilding*>, allocator<pair<CThingBuilding* const, list<CThingAICreature*> > > >::_M_insert(_Rb_tree_node_base*, _Rb_tree_node_base*, const pair<CThingBuilding* const, list<CThingAICreature*> >&, _Rb_tree_node_base*);
}
