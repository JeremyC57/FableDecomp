// std::_Rb_tree<CCharString, pair<const CCharString, CCountedPointer<NUISystem::CComponent> >, ...>::_M_erase(_Rb_tree_node*) -- STLport 4.6 tree erase without rebalancing of NUISystem::CManager's map<CCharString, CCountedPointer<CComponent> > (BastardChildren): recurse right, destroy the node's pair (out-of-line ~pair 0x00429E16), free the node, continue left
// Leaf: ?_M_erase@?$_Rb_tree@VCCharString@@U?$pair@$$CBVCCharString@@V?$CCountedPointer@VCComponent@NUISystem@@@@@std@@U?$_Select1st@U?$pair@$$CBVCCharString@@V?$CCountedPointer@VCComponent@NUISystem@@@@@std@@@3@U?$less@VCCharString@@@3@V?$allocator@U?$pair@$$CBVCCharString@@V?$CCountedPointer@VCComponent@NUISystem@@@@@std@@@3@@std@@AAEXPAU?$_Rb_tree_node@U?$pair@$$CBVCCharString@@V?$CCountedPointer@VCComponent@NUISystem@@@@@std@@@2@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0042B8FC, 53 bytes, thiscall, `ret 4`, size-optimised TU.
// Name (NF229, 2026-11-22, q12608): the STLport spelling the header produces (STL instances have no PDB names: EXTRA_RULES 2026-11-20
//   "descriptive-by-library"). Its landed binder, the tree's clear 0x0042C02F (nf228), binds it under this spelling.
//   Was the placeholder ?BuildTreeArray@CTextTreeWalkThrough@@QAEXPAVTreeNode@@@Z (catalog CEngineInternalPrimitiveBase::ClearPrimitiveList,
//   a byte-twin label; local TreeNode / SubObject models).
// Model: engine/NUISystem_CManager.h (the member's map type) and stlport/fable_map.h (STLport _tree.c _M_erase); the explicit
//   instantiation emits the member as its own COMDAT (the nf228 0x0042C02F pattern). Defines and pragma as 0x0042C02F (same retail TU).
#define FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA
#pragma optimize("s", on)
#include "engine/CCountedPointer.h"
#include "engine/NUISystem_CManager.h"
template void std::_Rb_tree<CCharString, std::pair<const CCharString, CCountedPointer<NUISystem::CComponent> >, std::_Select1st<std::pair<const CCharString, CCountedPointer<NUISystem::CComponent> > >, std::less<CCharString>, std::allocator<std::pair<const CCharString, CCountedPointer<NUISystem::CComponent> > > >::_M_erase(std::_Rb_tree_node<std::pair<const CCharString, CCountedPointer<NUISystem::CComponent> > >*);
