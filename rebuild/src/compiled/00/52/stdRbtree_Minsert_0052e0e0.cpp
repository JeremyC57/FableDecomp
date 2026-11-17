// std::_Rb_tree<EEvent, EEvent, _Identity, less<EEvent> >::_M_insert(_Rb_tree_node_base* x, _Rb_tree_node_base* y, const EEvent& v, _Rb_tree_node_base* w) -- STLport red-black tree node insert of NUISystem::CObserver's observed-event set (EEvent = NUISystem::EEvent): allocate a node, copy the event in, link it left/right of y (maintaining root/leftmost/rightmost), rebalance, count, return the iterator
// Retail: Fable.exe (TLC, VC7.1) @ 0x0052E0E0, 179 bytes, `ret 14h` (thiscall: the hidden iterator result, x, y, &v, w).
// Callees: malloc 0x00BFEA0E (the 0x14-byte node: 0x10 links + the 4-byte event), _Rb_global<bool>::_Rebalance(node,
//   root&) 0x0042971D (static, __cdecl).
// Callers: the set's insert_unique 0x0052E230 (landed: `Insert(&j, x, y, value, 0)` at 0x0052E283 / 0x0052E2BB), used by
//   NUISystem::CObserver::ObserveEvent 0x0052DA20 (landed).
// Label correction: catalogued as "CRedBlackTree_Insert_Int"; it is STLport `_Rb_tree::_M_insert` of the
//   set<NUISystem::EEvent> CObserver::EventsToObserve (Ego_r.pdb CObserver +0x04).
// Layout: STLport `_Rb_tree` = { _M_header (node pointer) +0, _M_node_count +4, empty comparator +8 }; nodes =
//   { _M_color +0, _M_parent +4, _M_left +8, _M_right +0xc, value +0x10 }; header parent = root, left = leftmost,
//   right = rightmost. The enum compare is signed (`jl`).
// Shape: STLport 4.x `_M_insert` verbatim, `_M_create_node` (malloc + null-tested placement copy) inlined in both arms.
// NF50 (round 7): moved onto the shared stlport/fable_map.h as the explicit instantiation of the STLport 4.6.2 _tree.c
// _M_insert body (FABLE_RETAIL_STLP_MAP_ORIGINAL_INSERT_BODY, the landed 0x0042CB08 pattern): the rebalance call binds
// ?_Rebalance@?$_Rb_global@_N@std@@SAXPAU_Rb_tree_node_base@2@AAPAU32@@Z and the row's own symbol is the STLport _M_insert its landed
// caller already binds (descriptive-by-library: both PDBs are Dinkumware builds; template arguments from the caller's binding).
// Leaf: ?_M_insert@?$_Rb_tree@W4EEvent@NUISystem@@W412@U?$_Identity@W4EEvent@NUISystem@@@std@@U?$less@W4EEvent@NUISystem@@@4@V?$allocator@W4EEvent@NUISystem@@@4@@std@@AAE?AU?$_Rb_tree_iterator@W4EEvent@NUISystem@@U?$_Nonconst_traits@W4EEvent@NUISystem@@@std@@@2@PAU_Rb_tree_node_base@2@0ABW4EEvent@NUISystem@@0@Z
#define FABLE_RETAIL_STLP_MAP_ORIGINAL_INSERT_BODY
#include "engine/NUISystem_enums.h"
#include "stlport/fable_map.h"
namespace std {
template _Rb_tree<NUISystem::EEvent, NUISystem::EEvent, _Identity<NUISystem::EEvent >, less<NUISystem::EEvent >, allocator<NUISystem::EEvent > >::iterator _Rb_tree<NUISystem::EEvent, NUISystem::EEvent, _Identity<NUISystem::EEvent >, less<NUISystem::EEvent >, allocator<NUISystem::EEvent > >::_M_insert(_Rb_tree_node_base*, _Rb_tree_node_base*, const NUISystem::EEvent&, _Rb_tree_node_base*);
}
