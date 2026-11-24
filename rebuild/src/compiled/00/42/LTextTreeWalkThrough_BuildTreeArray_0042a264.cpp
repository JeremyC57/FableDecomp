// std::_Rb_tree<long, pair<const long, EEngine2DLayer>, ...>::_M_erase(_Rb_tree_node*) -- STLport 4.6 tree erase without rebalancing of NUISystem::CManager's map<long, EEngine2DLayer> (the meta-layer map): recurse right, free each node (the pair has a trivial destructor), continue left
// Leaf: ?_M_erase@?$_Rb_tree@JU?$pair@$$CBJW4EEngine2DLayer@@@std@@U?$_Select1st@U?$pair@$$CBJW4EEngine2DLayer@@@std@@@2@U?$less@J@2@V?$allocator@U?$pair@$$CBJW4EEngine2DLayer@@@std@@@2@@std@@AAEXPAU?$_Rb_tree_node@U?$pair@$$CBJW4EEngine2DLayer@@@std@@@2@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0042A264, 45 bytes, thiscall, `ret 4`, size-optimised TU.
// Name (NF229, 2026-11-22, q12609): the STLport spelling the header produces (STL instances have no PDB names: EXTRA_RULES 2026-11-20
//   "descriptive-by-library"). Its landed binder, the tree's clear 0x0042ACC3 (nf228), binds it under this spelling.
//   Was the placeholder ?BuildTreeArray@Tree@@QAEXPAUNode@@@Z, shared with 0x0042A237 (LNK4006) (catalog LTextBinTree<LTextGroup*>::LTextTreeWalkThrough::BuildTreeArray; a local model).
// Model: engine/NUISystem_CManager.h (the member's map type) and stlport/fable_map.h (STLport _tree.c _M_erase); the explicit instantiation emits the
//   member as its own COMDAT (the nf228 0x0042C02F pattern). Defines and pragma as the tree's clear 0x0042ACC3 (same retail TU).
#define FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA
#pragma optimize("s", on)
#include "engine/CCountedPointer.h"
#include "engine/NUISystem_CManager.h"
template void std::_Rb_tree<long, std::pair<const long, EEngine2DLayer >, std::_Select1st<std::pair<const long, EEngine2DLayer > >, std::less<long>, std::allocator<std::pair<const long, EEngine2DLayer > > >::_M_erase(std::_Rb_tree_node<std::pair<const long, EEngine2DLayer > >*);
