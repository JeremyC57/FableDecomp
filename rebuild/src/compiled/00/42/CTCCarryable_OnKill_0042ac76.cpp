// std::_Rb_tree<NUISystem::CManager::EUsedKeys, pair<const EUsedKeys, long >, ...>::clear() -- STLport 4.6 tree clear of NUISystem::CManager's map<EUsedKeys, long > (UsedKeys): when the tree is not empty, erase every node from the root (_M_erase, out of line), then reset the header node and the node count
// Leaf: ?clear@?$_Rb_tree@W4EUsedKeys@CManager@NUISystem@@U?$pair@$$CBW4EUsedKeys@CManager@NUISystem@@J@std@@U?$_Select1st@U?$pair@$$CBW4EUsedKeys@CManager@NUISystem@@J@std@@@5@U?$less@W4EUsedKeys@CManager@NUISystem@@@5@V?$allocator@U?$pair@$$CBW4EUsedKeys@CManager@NUISystem@@J@std@@@5@@std@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0042AC76, 41 bytes, size-optimised TU.
// Name (NF228, 2026-11-22, q12594): the STLport spelling the header produces (STL instances have no PDB names: EXTRA_RULES 2026-11-20
//   "descriptive-by-library"); the ~_Rb_tree 0x0042B796 (a call) and map() 0x0042BF67 (its pick-any ~_Rb_tree copy) bind it; their pick-any copies of this member equal this address (relocations alike).
//   Was the placeholder ?OnKill@T@@QAEXXZ (catalog CTCCarryable::OnKill, a byte-twin label of the len-41 family; a local `struct T` model).
// Model: engine/NUISystem_CManager.h (the member's map type) and stlport/fable_map.h (the default-path inline member); the explicit
//   instantiation emits the member as its own COMDAT (the 0x008353B0 pattern). Defines and pragma as 0x0042B796 (the same retail TU).
#define FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA
#pragma optimize("s", on)
#include "engine/CCountedPointer.h"
#include "engine/NUISystem_CManager.h"
template void std::_Rb_tree<NUISystem::CManager::EUsedKeys, std::pair<const NUISystem::CManager::EUsedKeys, long >, std::_Select1st<std::pair<const NUISystem::CManager::EUsedKeys, long > >, std::less<NUISystem::CManager::EUsedKeys>, std::allocator<std::pair<const NUISystem::CManager::EUsedKeys, long > > >::clear();
