// std::_Rb_tree<NUISystem::CManager::EUsedKeys, pair<const EUsedKeys, long >, ...>::_Rb_tree(const less&, const allocator&) -- STLport 4.6 tree constructor of NUISystem::CManager's map<EUsedKeys, long >: header node allocated (_Rb_tree_base), node count 0, then _M_empty_initialize (colour red, root 0, leftmost = rightmost = header)
// Leaf: ??0?$_Rb_tree@W4EUsedKeys@CManager@NUISystem@@U?$pair@$$CBW4EUsedKeys@CManager@NUISystem@@J@std@@U?$_Select1st@U?$pair@$$CBW4EUsedKeys@CManager@NUISystem@@J@std@@@5@U?$less@W4EUsedKeys@CManager@NUISystem@@@5@V?$allocator@U?$pair@$$CBW4EUsedKeys@CManager@NUISystem@@J@std@@@5@@std@@QAE@ABU?$less@W4EUsedKeys@CManager@NUISystem@@@1@ABV?$allocator@U?$pair@$$CBW4EUsedKeys@CManager@NUISystem@@J@std@@@1@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0042B76C, 42 bytes, size-optimised TU.
// Name (NF228, 2026-11-22, q12595): the STLport spelling the header produces (STL instances have no PDB names: EXTRA_RULES 2026-11-20
//   "descriptive-by-library"); its only landed binder, map() 0x0042BF67, calls it under this spelling (its pick-any copy of this member equals this address).
//   Was the placeholder ?CTexture_InitEmpty_24bytes@NodeInitOwner_0042b76c@@... (catalog CTexture_InitEmpty_24bytes, a row-local stand-in struct).
// Model: engine/NUISystem_CManager.h (the member's map type) and stlport/fable_map.h (the default-path inline member); the explicit
//   instantiation emits the member as its own COMDAT (the 0x008353B0 pattern). Defines and pragma as 0x0042BF67 (the same retail TU).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA
#define FABLE_RETAIL_STLP_MAP
#pragma optimize("s", on)
#include "engine/CCountedPointer.h"
#include "engine/NUISystem_CManager.h"
template std::_Rb_tree<NUISystem::CManager::EUsedKeys, std::pair<const NUISystem::CManager::EUsedKeys, long >, std::_Select1st<std::pair<const NUISystem::CManager::EUsedKeys, long > >, std::less<NUISystem::CManager::EUsedKeys>, std::allocator<std::pair<const NUISystem::CManager::EUsedKeys, long > > >::_Rb_tree(const std::less<NUISystem::CManager::EUsedKeys>&, const std::allocator<std::pair<const NUISystem::CManager::EUsedKeys, long > >&);
