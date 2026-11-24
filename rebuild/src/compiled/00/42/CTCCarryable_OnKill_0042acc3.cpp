// std::_Rb_tree<long, pair<const long, EEngine2DLayer >, ...>::clear() -- STLport 4.6 tree clear of NUISystem::CManager's map<long, EEngine2DLayer > (Layers +0x70): when the tree is not empty, erase every node from the root (_M_erase, out of line), then reset the header node and the node count
// Leaf: ?clear@?$_Rb_tree@JU?$pair@$$CBJW4EEngine2DLayer@@@std@@U?$_Select1st@U?$pair@$$CBJW4EEngine2DLayer@@@std@@@2@U?$less@J@2@V?$allocator@U?$pair@$$CBJW4EEngine2DLayer@@@std@@@2@@std@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0042ACC3, 41 bytes, size-optimised TU.
// Name (NF228, 2026-11-22, q12593): the STLport spelling the header produces (STL instances have no PDB names: EXTRA_RULES 2026-11-20
//   "descriptive-by-library"); the ~_Rb_tree 0x0042B7D7 (a call) and map() 0x0042BF85 (its pick-any ~_Rb_tree copy) bind it; their pick-any copies of this member equal this address (relocations alike).
//   Was the placeholder ?OnKill@T@@QAEXXZ (catalog CTCCarryable::OnKill, a byte-twin label of the len-41 family; a local `struct T` model).
// Model: engine/NUISystem_CManager.h (the member's map type) and stlport/fable_map.h (the default-path inline member); the explicit
//   instantiation emits the member as its own COMDAT (the 0x008353B0 pattern). Defines and pragma as 0x0042B7D7 (the same retail TU).
#define FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA
#pragma optimize("s", on)
#include "engine/CCountedPointer.h"
#include "engine/NUISystem_CManager.h"
template void std::_Rb_tree<long, std::pair<const long, EEngine2DLayer >, std::_Select1st<std::pair<const long, EEngine2DLayer > >, std::less<long>, std::allocator<std::pair<const long, EEngine2DLayer > > >::clear();
