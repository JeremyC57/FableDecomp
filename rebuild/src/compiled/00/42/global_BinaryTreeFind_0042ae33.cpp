// std::_Rb_tree<long, pair<const long, EEngine2DLayer>, ...>::_M_lower_bound(const long&) const -- STLport 4.6 lower-bound walk of NUISystem::CManager's map<long, EEngine2DLayer> (the meta-layer map): the last node whose key is not less than k (the header when none)
// Leaf: ?_M_lower_bound@?$_Rb_tree@JU?$pair@$$CBJW4EEngine2DLayer@@@std@@U?$_Select1st@U?$pair@$$CBJW4EEngine2DLayer@@@std@@@2@U?$less@J@2@V?$allocator@U?$pair@$$CBJW4EEngine2DLayer@@@std@@@2@@std@@ABEPAU_Rb_tree_node_base@2@ABJ@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0042AE33, 37 bytes, thiscall, `ret 4`; returns the node pointer.
// Name (NF229, 2026-11-22, q12614): the STLport spelling the header produces (STL instances have no PDB names: EXTRA_RULES 2026-11-20
//   "descriptive-by-library"). Its landed binder, map<long, EEngine2DLayer>::operator[] 0x0042D246 (nf228,
//   FABLE_RETAIL_STLP_TREE_BOUNDS_OUTOFLINE), binds it under this spelling; 0x0041E1CD SetMetaLayer's pick-any copy (BOUNDS_BODY_PASS22)
//   equals this body. Was the placeholder ?BinarySearchTree_FindByIntKey@@YIPAUNode@@PAUTree@@PAXPAH@Z (fastcall; catalog BinaryTree_Find; local Node / Tree models).
// Model: engine/NUISystem_CManager.h (the member's map type) and stlport/fable_map.h (FABLE_RETAIL_STLP_TREE_BOUNDS_OUTOFLINE +
//   _BOUNDS_BODY_PASS22: the STLport _tree.c walk); the explicit instantiation emits the member as its own COMDAT (the 0x004B9D00 pattern).
//   Defines and pragma as 0x0042D246 (same retail TU) plus the visible body.
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_STLP_TREE_BOUNDS_OUTOFLINE
#define FABLE_RETAIL_STLP_TREE_BOUNDS_BODY_PASS22
#pragma optimize("s", on)
#include "engine/NUISystem_CManager.h"
template std::_Rb_tree_node_base* std::_Rb_tree<long, std::pair<const long, EEngine2DLayer >, std::_Select1st<std::pair<const long, EEngine2DLayer > >, std::less<long>, std::allocator<std::pair<const long, EEngine2DLayer > > >::_M_lower_bound(const long&) const;
