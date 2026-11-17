// std::_Rb_tree<unsigned __int64, pair<const unsigned __int64, CThing*>, _Select1st, less<unsigned __int64> >::_M_insert(_Rb_tree_node_base* x, _Rb_tree_node_base* y, const value_type& v, _Rb_tree_node_base* w) -- STLport red-black tree node insert of CThingManager::UIDToThingMap: allocate a node, copy the value in, link it left/right of y (maintaining root/leftmost/rightmost), rebalance, count, return the iterator
// Retail: Fable.exe (TLC, VC7.1) @ 0x005292F0, 215 bytes, `ret 14h` (thiscall: the hidden iterator result, x, y, &v, w).
// Callees: malloc 0x00BFEA0E (the 0x20-byte node: 0x10 links + the 16-byte, 8-aligned pair),
//   _Rb_global<bool>::_Rebalance(node, root&) 0x0042971D (static, __cdecl).
// Callers: the tree's insert_unique pair at 0x0052A25B / 0x0052A29D (CThingManager TU, 0x0052xxxx).
// Layout: STLport `_Rb_tree` = { _M_header (node pointer) +0, _M_node_count +4 }; the header's parent is the root
//   (+4), its left the leftmost (+8), its right the rightmost (+0xc); the value sits at node +0x10: the unsigned
//   64-bit UID then the CThing pointer (only the three used words are copied). The PDB names the map
//   CThingManager::UIDToThingMap (map<unsigned __int64, CThing*>).
// Shape: byte twin of the landed CThingCompareMap::Insert 0x006F1280 (same instantiation compiled in another TU):
//   STLport's _M_insert condition `y == header || (w == 0 && (x != 0 || key(v) < key(y)))`; _M_create_node is the
//   placement-new construct with its null test, duplicated in both branches; the pair's memberwise copy
//   constructor gives the three edx word moves; the unsigned 64-bit `<` is `jb/ja` on the high word then the low.
// NF50 (round 7): moved onto the shared stlport/fable_map.h as the explicit instantiation of the STLport 4.6.2 _tree.c
// _M_insert body (FABLE_RETAIL_STLP_MAP_ORIGINAL_INSERT_BODY, the landed 0x0042CB08 pattern): the rebalance call binds
// ?_Rebalance@?$_Rb_global@_N@std@@SAXPAU_Rb_tree_node_base@2@AAPAU32@@Z and the row's own symbol is the STLport _M_insert its landed
// caller already binds (descriptive-by-library: both PDBs are Dinkumware builds; template arguments from the caller's binding).
// Leaf: ?_M_insert@?$_Rb_tree@_KU?$pair@$$CB_KPAVCThing@@@std@@U?$_Select1st@U?$pair@$$CB_KPAVCThing@@@std@@@2@U?$less@_K@2@V?$allocator@U?$pair@$$CB_KPAVCThing@@@std@@@2@@std@@AAE?AU?$_Rb_tree_iterator@U?$pair@$$CB_KPAVCThing@@@std@@U?$_Nonconst_traits@U?$pair@$$CB_KPAVCThing@@@std@@@2@@2@PAU_Rb_tree_node_base@2@0ABU?$pair@$$CB_KPAVCThing@@@2@0@Z
class CThing;
#define FABLE_RETAIL_STLP_MAP_ORIGINAL_INSERT_BODY
#include "stlport/fable_map.h"
namespace std {
template _Rb_tree<unsigned __int64, pair<const unsigned __int64, CThing*>, _Select1st<pair<const unsigned __int64, CThing*> >, less<unsigned __int64 >, allocator<pair<const unsigned __int64, CThing*> > >::iterator _Rb_tree<unsigned __int64, pair<const unsigned __int64, CThing*>, _Select1st<pair<const unsigned __int64, CThing*> >, less<unsigned __int64 >, allocator<pair<const unsigned __int64, CThing*> > >::_M_insert(_Rb_tree_node_base*, _Rb_tree_node_base*, const pair<const unsigned __int64, CThing*>&, _Rb_tree_node_base*);
}
