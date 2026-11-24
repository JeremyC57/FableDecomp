// std::_Rb_tree<NUISystem::CManager::EUsedKeys, pair<const NUISystem::CManager::EUsedKeys, long >, ...>::~_Rb_tree() -- STLport 4.6 tree destructor of NUISystem::CManager's map<NUISystem::CManager::EUsedKeys, long > (Keys +0x24): clear() (out of line), then free the header node (_Rb_tree_base / alloc_proxy)
// Leaf: ??1?$_Rb_tree@W4EUsedKeys@CManager@NUISystem@@U?$pair@$$CBW4EUsedKeys@CManager@NUISystem@@J@std@@U?$_Select1st@U?$pair@$$CBW4EUsedKeys@CManager@NUISystem@@J@std@@@5@U?$less@W4EUsedKeys@CManager@NUISystem@@@5@V?$allocator@U?$pair@$$CBW4EUsedKeys@CManager@NUISystem@@J@std@@@5@@std@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0042B796, 23 bytes, size-optimised TU (`pop ecx` after free).
// Name (NF227, 2026-11-22, q12576): the STLport spelling the header produces (STL instances have no PDB names: EXTRA_RULES 2026-11-20
//   "descriptive-by-library"). NF226's port of ~CManager 0x0041E641 calls it for Keys +0x24 and its pick-any copy equals this address
//   (relocations alike); 0x0041E641 binds this spelling. Was the placeholder ??1list_T_0042b796@std@@QAE@XZ (catalog std::list<T_0042b796>::~list,
//   the node type "not recoverable from the bytes"; first line was: std::list<T_0042b796>::~list() -- STLport list destructor: clear the nodes (out-of-line _List_base::clear @ 0x0042AC7...).
// Model: engine/NUISystem_CManager.h (the member's map type) and stlport/fable_map.h; the explicit instantiation emits the member as
//   its own COMDAT (no /OPT:REF in retail), the 0x008353B0 / 0x008353F0 pattern. Defines and pragma as 0x0041E641 (the same retail TU).
#define FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA
#pragma optimize("s", on)
#include "engine/CCountedPointer.h"
#include "engine/NUISystem_CManager.h"
template std::_Rb_tree<NUISystem::CManager::EUsedKeys, std::pair<const NUISystem::CManager::EUsedKeys, long >, std::_Select1st<std::pair<const NUISystem::CManager::EUsedKeys, long > >, std::less<NUISystem::CManager::EUsedKeys>, std::allocator<std::pair<const NUISystem::CManager::EUsedKeys, long > > >::~_Rb_tree();
