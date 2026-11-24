// std::_Rb_tree<long, pair<const long, EEngine2DLayer >, ...>::~_Rb_tree() -- STLport 4.6 tree destructor of NUISystem::CManager's map<long, EEngine2DLayer > (Layers +0x70): clear() (out of line), then free the header node (_Rb_tree_base / alloc_proxy)
// Leaf: ??1?$_Rb_tree@JU?$pair@$$CBJW4EEngine2DLayer@@@std@@U?$_Select1st@U?$pair@$$CBJW4EEngine2DLayer@@@std@@@2@U?$less@J@2@V?$allocator@U?$pair@$$CBJW4EEngine2DLayer@@@std@@@2@@std@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0042B7D7, 23 bytes, size-optimised TU (`pop ecx` after free).
// Name (NF227, 2026-11-22, q12576): the STLport spelling the header produces (STL instances have no PDB names: EXTRA_RULES 2026-11-20
//   "descriptive-by-library"). NF226's port of ~CManager 0x0041E641 calls it for Layers +0x70 and its pick-any copy equals this address
//   (relocations alike); 0x0041E641 binds this spelling. Was the placeholder ??1list_T_0042b7d7@std@@QAE@XZ (catalog std::list<T_0042b7d7>::~list,
//   the node type "not recoverable from the bytes"; first line was: std::list<T_0042b7d7>::~list() -- STLport list destructor: clear the nodes (out-of-line _List_base::clear @ 0x0042ACC...).
// Model: engine/NUISystem_CManager.h (the member's map type) and stlport/fable_map.h; the explicit instantiation emits the member as
//   its own COMDAT (no /OPT:REF in retail), the 0x008353B0 / 0x008353F0 pattern. Defines and pragma as 0x0041E641 (the same retail TU).
#define FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA
#pragma optimize("s", on)
#include "engine/CCountedPointer.h"
#include "engine/NUISystem_CManager.h"
template std::_Rb_tree<long, std::pair<const long, EEngine2DLayer >, std::_Select1st<std::pair<const long, EEngine2DLayer > >, std::less<long>, std::allocator<std::pair<const long, EEngine2DLayer > > >::~_Rb_tree();
