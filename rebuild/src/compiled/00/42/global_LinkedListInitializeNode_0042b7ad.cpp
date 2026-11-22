// std::_Rb_tree<long, pair<const long, EEngine2DLayer >, ...>::_Rb_tree(const less&, const allocator&) -- STLport 4.6 tree constructor of NUISystem::CManager's map<long, EEngine2DLayer > (Layers +0x70): header node allocated (_Rb_tree_base), node count 0, then _M_empty_initialize
// Leaf: ??0?$_Rb_tree@JU?$pair@$$CBJW4EEngine2DLayer@@@std@@U?$_Select1st@U?$pair@$$CBJW4EEngine2DLayer@@@std@@@2@U?$less@J@2@V?$allocator@U?$pair@$$CBJW4EEngine2DLayer@@@std@@@2@@std@@QAE@ABU?$less@J@1@ABV?$allocator@U?$pair@$$CBJW4EEngine2DLayer@@@std@@@1@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0042B7AD, 42 bytes, size-optimised TU.
// Name (NF228, 2026-11-22, q12595): the STLport spelling the header produces (STL instances have no PDB names: EXTRA_RULES 2026-11-20
//   "descriptive-by-library"); its only landed binder, map() 0x0042BF85, calls it under this spelling (its pick-any copy of this member equals this address).
//   Was the placeholder ?LinkedList_InitializeNode@NodeInitOwner_0042b7ad@@... (catalog LinkedList_InitializeNode, a row-local stand-in struct).
// Model: engine/NUISystem_CManager.h (the member's map type) and stlport/fable_map.h (the default-path inline member); the explicit
//   instantiation emits the member as its own COMDAT (the 0x008353B0 pattern). Defines and pragma as 0x0042BF85 (the same retail TU).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA
#define FABLE_RETAIL_STLP_MAP
#pragma optimize("s", on)
#include "engine/CCountedPointer.h"
#include "engine/NUISystem_CManager.h"
template std::_Rb_tree<long, std::pair<const long, EEngine2DLayer >, std::_Select1st<std::pair<const long, EEngine2DLayer > >, std::less<long>, std::allocator<std::pair<const long, EEngine2DLayer > > >::_Rb_tree(const std::less<long>&, const std::allocator<std::pair<const long, EEngine2DLayer > >&);
