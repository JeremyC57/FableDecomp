// std::_Rb_tree_base<std::pair<const NUISystem::CManager::EUsedKeys, long >, std::allocator<std::pair<const NUISystem::CManager::EUsedKeys, long > > >::_Rb_tree_base(const allocator&) -- STLport 4.6 tree base constructor of NUISystem::CManager's map<EUsedKeys, long> (UsedKeys): clear the header-node pointer, then allocate the header node (malloc(0x18))
// Leaf: ??0?$_Rb_tree_base@U?$pair@$$CBW4EUsedKeys@CManager@NUISystem@@J@std@@V?$allocator@U?$pair@$$CBW4EUsedKeys@CManager@NUISystem@@J@std@@@2@@std@@QAE@ABV?$allocator@U?$pair@$$CBW4EUsedKeys@CManager@NUISystem@@J@std@@@1@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0042AC9F, 22 bytes, thiscall, `ret 4`, size-optimised TU (`and dword ptr [esi],0`, malloc(0x18)).
// Name (NF229, 2026-11-22, q12611): the STLport spelling the header produces (STL instances have no PDB names: EXTRA_RULES 2026-11-20
//   "descriptive-by-library"). Its landed binder, the _Rb_tree(const less&, const allocator&) constructor 0x0042B76C (nf228), binds it under
//   this spelling. Was the placeholder ?Allocator_Alloc_24bytes@@YIPAPAXPAPAXPAXPBX@Z (fastcall, catalog Allocator_Alloc_24bytes; no model).
// Model: engine/NUISystem_CManager.h (the member's map type) and stlport/fable_map.h (STLport _tree.h _Rb_tree_base); the explicit
//   instantiation emits the member as its own COMDAT (the nf228 0x0042B76C pattern). Defines and pragma as 0x0042B76C (same retail TU).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA
#define FABLE_RETAIL_STLP_MAP
#pragma optimize("s", on)
#include "engine/CCountedPointer.h"
#include "engine/NUISystem_CManager.h"
template std::_Rb_tree_base<std::pair<const NUISystem::CManager::EUsedKeys, long >, std::allocator<std::pair<const NUISystem::CManager::EUsedKeys, long > > >::_Rb_tree_base(const std::allocator<std::pair<const NUISystem::CManager::EUsedKeys, long > >&);
