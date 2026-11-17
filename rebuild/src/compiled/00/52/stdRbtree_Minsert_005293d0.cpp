// std::_Rb_tree<unsigned __int64, pair<const unsigned __int64, pair<CCharString, CWideString> >, _Select1st, less<unsigned __int64> >::_M_insert(_Rb_tree_node_base* x, _Rb_tree_node_base* y, const value_type& v, _Rb_tree_node_base* w) -- STLport red-black tree node insert of CThingManager::NPCNameMap: allocate a node, copy-construct the UID and the (name, display name) pair in, link it left/right of y (maintaining root/leftmost/rightmost), rebalance, count, return the iterator
// Retail: Fable.exe (TLC, VC7.1) @ 0x005293D0, 214 bytes, `ret 14h` (thiscall: the hidden iterator result, x, y, &v, w).
// Callees: malloc 0x00BFEA0E (the 0x20-byte node: 0x10 links + the 8-byte UID + CCharString + CWideString),
//   _M_create_node 0x005289C0 (the out-of-line copy of the inline member; manifest "CSplineEntry_Create" is
//   propagated), CCharString copy ctor 0x0099EC30, CWideString copy ctor 0x0099B720, _Rb_global<bool>::_Rebalance(node,
//   root&) 0x0042971D (static, __cdecl).
// Callers: the tree's insert_unique / insert(hint) bodies 0x0052950B .. 0x0052A4D7 (nine call sites).
// Label correction: catalogued as "ListNode_InsertSpline"; it is STLport `_Rb_tree::_M_insert` of
//   map<unsigned __int64, pair<CCharString, CWideString> > (Ego_r.pdb CThingManager::NPCNameMap).
// Layout: STLport `_Rb_tree` = { _M_header (node pointer) +0, _M_node_count +4 }; header parent = root (+4), left =
//   leftmost (+8), right = rightmost (+0xc); the value at node +0x10: UID, then CCharString +0x18, CWideString +0x1c.
// Shape: the shared STLport `_M_insert` source; VC7.1 inlines `_M_create_node` in the left arm and calls the
//   out-of-line copy from the right arm (inline-once band). STLport's pair copy constructors are memberwise: the key
//   words, then the inner pair's two string copy constructors; the inner pair's `this` rides in the dead `y` argument
//   slot across the first call (`mov [esp+20h],eax` / `mov ecx,[esp+1Ch] ; add ecx,4`).
// NF50 (round 7): moved onto the shared stlport/fable_map.h as the explicit instantiation of the STLport 4.6.2 _tree.c
// _M_insert body (FABLE_RETAIL_STLP_MAP_ORIGINAL_INSERT_BODY, the landed 0x0042CB08 pattern): the rebalance call binds
// ?_Rebalance@?$_Rb_global@_N@std@@SAXPAU_Rb_tree_node_base@2@AAPAU32@@Z and the row's own symbol is the STLport _M_insert its landed
// caller already binds (descriptive-by-library: both PDBs are Dinkumware builds; template arguments from the caller's binding).
// Leaf: ?_M_insert@?$_Rb_tree@_KU?$pair@$$CB_KU?$pair@VCCharString@@VCWideString@@@std@@@std@@U?$_Select1st@U?$pair@$$CB_KU?$pair@VCCharString@@VCWideString@@@std@@@std@@@2@U?$less@_K@2@V?$allocator@U?$pair@$$CB_KU?$pair@VCCharString@@VCWideString@@@std@@@std@@@2@@std@@AAE?AU?$_Rb_tree_iterator@U?$pair@$$CB_KU?$pair@VCCharString@@VCWideString@@@std@@@std@@U?$_Nonconst_traits@U?$pair@$$CB_KU?$pair@VCCharString@@VCWideString@@@std@@@std@@@2@@2@PAU_Rb_tree_node_base@2@0ABU?$pair@$$CB_KU?$pair@VCCharString@@VCWideString@@@std@@@2@0@Z
#define FABLE_RETAIL_STLP_MAP_ORIGINAL_INSERT_BODY
#include "fable_string.h"
#include "stlport/fable_map.h"
namespace std {
template _Rb_tree<unsigned __int64, pair<const unsigned __int64, pair<CCharString, CWideString> >, _Select1st<pair<const unsigned __int64, pair<CCharString, CWideString> > >, less<unsigned __int64 >, allocator<pair<const unsigned __int64, pair<CCharString, CWideString> > > >::iterator _Rb_tree<unsigned __int64, pair<const unsigned __int64, pair<CCharString, CWideString> >, _Select1st<pair<const unsigned __int64, pair<CCharString, CWideString> > >, less<unsigned __int64 >, allocator<pair<const unsigned __int64, pair<CCharString, CWideString> > > >::_M_insert(_Rb_tree_node_base*, _Rb_tree_node_base*, const pair<const unsigned __int64, pair<CCharString, CWideString> >&, _Rb_tree_node_base*);
}
