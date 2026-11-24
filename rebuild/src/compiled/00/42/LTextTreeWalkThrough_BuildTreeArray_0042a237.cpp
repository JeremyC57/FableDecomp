// std::_Rb_tree<NUISystem::CManager::EUsedKeys, pair<const EUsedKeys, long>, ...>::_M_erase(_Rb_tree_node*) -- STLport 4.6 tree erase without rebalancing of NUISystem::CManager's map<EUsedKeys, long> (UsedKeys): recurse right, free each node (the pair has a trivial destructor), continue left
// Leaf: ?_M_erase@?$_Rb_tree@W4EUsedKeys@CManager@NUISystem@@U?$pair@$$CBW4EUsedKeys@CManager@NUISystem@@J@std@@U?$_Select1st@U?$pair@$$CBW4EUsedKeys@CManager@NUISystem@@J@std@@@5@U?$less@W4EUsedKeys@CManager@NUISystem@@@5@V?$allocator@U?$pair@$$CBW4EUsedKeys@CManager@NUISystem@@J@std@@@5@@std@@AAEXPAU?$_Rb_tree_node@U?$pair@$$CBW4EUsedKeys@CManager@NUISystem@@J@std@@@2@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0042A237, 45 bytes, thiscall, `ret 4`, size-optimised TU.
// Name (NF229, 2026-11-22, q12610): the STLport spelling the header produces (STL instances have no PDB names: EXTRA_RULES 2026-11-20
//   "descriptive-by-library"). Its landed binder, the tree's clear 0x0042AC76 (nf228), binds it under this spelling.
//   Was the placeholder ?BuildTreeArray@Tree@@QAEXPAUNode@@@Z, shared with 0x0042A264 (LNK4006) (catalog LTextBinTree<LTextGroup*>::LTextTreeWalkThrough::BuildTreeArray; a local model).
// Model: engine/NUISystem_CManager.h (the member's map type) and stlport/fable_map.h (STLport _tree.c _M_erase); the explicit instantiation emits the
//   member as its own COMDAT (the nf228 0x0042C02F pattern). Defines and pragma as the tree's clear 0x0042AC76 (same retail TU).
#define FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA
#pragma optimize("s", on)
#include "engine/CCountedPointer.h"
#include "engine/NUISystem_CManager.h"
template void std::_Rb_tree<NUISystem::CManager::EUsedKeys, std::pair<const NUISystem::CManager::EUsedKeys, long >, std::_Select1st<std::pair<const NUISystem::CManager::EUsedKeys, long > >, std::less<NUISystem::CManager::EUsedKeys>, std::allocator<std::pair<const NUISystem::CManager::EUsedKeys, long > > >::_M_erase(std::_Rb_tree_node<std::pair<const NUISystem::CManager::EUsedKeys, long > >*);
