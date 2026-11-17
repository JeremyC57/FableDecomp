// std::_Rb_tree<unsigned __int64, pair<const unsigned __int64, CPersistedDeadEntity>, _Select1st, less<unsigned __int64> >::_M_insert(_Rb_tree_node_base* x, _Rb_tree_node_base* y, const value_type& v, _Rb_tree_node_base* w) -- STLport red-black tree node insert of CEntityRuntimePersistence::PersistedDeadEntities: allocate a node, copy the value in, link it left/right of y (maintaining root/leftmost/rightmost), rebalance, count, return the iterator
// Retail: Fable.exe (TLC, VC7.1) @ 0x00529150, 210 bytes, `ret 14h` (thiscall: the hidden iterator result, x, y, &v, w).
// Callees: malloc 0x00BFEA0E (the 0x28-byte node: 0x10 links + the 8-byte UID + the 16-byte CPersistedDeadEntity),
//   _M_create_node 0x005288F0 (the out-of-line copy of the inline member, manifest "CCreatureAction_NavToPos::Clone"
//   is propagated), _Rb_global<bool>::_Rebalance(node, root&) 0x0042971D (static, __cdecl).
// Callers: the tree's insert_unique pair at 0x0052A18B / 0x0052A1CD (reached from the landed
//   CEntityRuntimePersistence::AddPersistedDeadEntity 0x00520330 through insert 0x0052A130).
// Label correction: catalogued as "CRedBlackTree_Insert_With_Clone"; it is STLport `_Rb_tree::_M_insert` of
//   map<unsigned __int64, CPersistedDeadEntity> (Ego_r.pdb CEntityRuntimePersistence::PersistedDeadEntities).
// Layout: STLport `_Rb_tree` = { _M_header (node pointer) +0, _M_node_count +4 }; header parent = root (+4), left =
//   leftmost (+8), right = rightmost (+0xc); the value sits at node +0x10: the UID key then the record
//   {UID +0, GamePersistent +8, SectionNameCRC +0xc}.
// Shape: the same STLport `_M_insert` source as the landed UIDToThingMap / CThingCompareMap inserts; with the larger
//   value copy VC7.1 inlines `_M_create_node` in the left arm only and calls its out-of-line copy (0x005288F0) from
//   the right arm (the compiler's inline-once band, nothing in the source differs). The pair's memberwise copy
//   constructor copies the key words, the POD record goes as a block through re-based pointers (`add esi,8 ;
//   add ecx,8` then four words).
// NF50 (round 7): moved onto the shared stlport/fable_map.h as the explicit instantiation of the STLport 4.6.2 _tree.c
// _M_insert body (FABLE_RETAIL_STLP_MAP_ORIGINAL_INSERT_BODY, the landed 0x0042CB08 pattern): the rebalance call binds
// ?_Rebalance@?$_Rb_global@_N@std@@SAXPAU_Rb_tree_node_base@2@AAPAU32@@Z and the row's own symbol is the STLport _M_insert its landed
// caller already binds (descriptive-by-library: both PDBs are Dinkumware builds; template arguments from the caller's binding).
// Leaf: ?_M_insert@?$_Rb_tree@_KU?$pair@$$CB_KVCPersistedDeadEntity@CEntityRuntimePersistence@NThingManager@@@std@@U?$_Select1st@U?$pair@$$CB_KVCPersistedDeadEntity@CEntityRuntimePersistence@NThingManager@@@std@@@2@U?$less@_K@2@V?$allocator@U?$pair@$$CB_KVCPersistedDeadEntity@CEntityRuntimePersistence@NThingManager@@@std@@@2@@std@@AAE?AU?$_Rb_tree_iterator@U?$pair@$$CB_KVCPersistedDeadEntity@CEntityRuntimePersistence@NThingManager@@@std@@U?$_Nonconst_traits@U?$pair@$$CB_KVCPersistedDeadEntity@CEntityRuntimePersistence@NThingManager@@@std@@@2@@2@PAU_Rb_tree_node_base@2@0ABU?$pair@$$CB_KVCPersistedDeadEntity@CEntityRuntimePersistence@NThingManager@@@2@0@Z
#define FABLE_RETAIL_STLP_MAP_ORIGINAL_INSERT_BODY
#include "engine/NThingManager_CEntityRuntimePersistence.h"
#include "stlport/fable_map.h"
namespace std {
template _Rb_tree<unsigned __int64, pair<const unsigned __int64, NThingManager::CEntityRuntimePersistence::CPersistedDeadEntity>, _Select1st<pair<const unsigned __int64, NThingManager::CEntityRuntimePersistence::CPersistedDeadEntity> >, less<unsigned __int64 >, allocator<pair<const unsigned __int64, NThingManager::CEntityRuntimePersistence::CPersistedDeadEntity> > >::iterator _Rb_tree<unsigned __int64, pair<const unsigned __int64, NThingManager::CEntityRuntimePersistence::CPersistedDeadEntity>, _Select1st<pair<const unsigned __int64, NThingManager::CEntityRuntimePersistence::CPersistedDeadEntity> >, less<unsigned __int64 >, allocator<pair<const unsigned __int64, NThingManager::CEntityRuntimePersistence::CPersistedDeadEntity> > >::_M_insert(_Rb_tree_node_base*, _Rb_tree_node_base*, const pair<const unsigned __int64, NThingManager::CEntityRuntimePersistence::CPersistedDeadEntity>&, _Rb_tree_node_base*);
}
