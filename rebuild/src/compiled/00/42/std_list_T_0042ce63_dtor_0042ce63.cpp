// std::_Rb_tree<CCharString, pair<const CCharString, CCountedPointer<NUISystem::CComponent> >, ...>::~_Rb_tree() -- STLport 4.6 tree destructor of NUISystem::CManager's map<CCharString, CCountedPointer<NUISystem::CComponent> > (BastardChildren +0x88): clear() (out of line), then free the header node (_Rb_tree_base / alloc_proxy)
// Leaf: ??1?$_Rb_tree@VCCharString@@U?$pair@$$CBVCCharString@@V?$CCountedPointer@VCComponent@NUISystem@@@@@std@@U?$_Select1st@U?$pair@$$CBVCCharString@@V?$CCountedPointer@VCComponent@NUISystem@@@@@std@@@3@U?$less@VCCharString@@@3@V?$allocator@U?$pair@$$CBVCCharString@@V?$CCountedPointer@VCComponent@NUISystem@@@@@std@@@3@@std@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0042CE63, 23 bytes, size-optimised TU (`pop ecx` after free).
// Name (NF227, 2026-11-22, q12576): the STLport spelling the header produces (STL instances have no PDB names: EXTRA_RULES 2026-11-20
//   "descriptive-by-library"). NF226's port of ~CManager 0x0041E641 calls it for BastardChildren +0x88 and its pick-any copy equals this address
//   (relocations alike); 0x0041E641 binds this spelling. Was the placeholder ??1list_T_0042ce63@std@@QAE@XZ (catalog std::list<T_0042ce63>::~list,
//   the node type "not recoverable from the bytes"; first line was: std::list<T_0042ce63>::~list() -- STLport list destructor: clear the nodes (out-of-line _List_base::clear @ 0x0042C02...).
// Model: engine/NUISystem_CManager.h (the member's map type) and stlport/fable_map.h; the explicit instantiation emits the member as
//   its own COMDAT (no /OPT:REF in retail), the 0x008353B0 / 0x008353F0 pattern. Defines and pragma as 0x0041E641 (the same retail TU).
#define FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA
#pragma optimize("s", on)
#include "engine/CCountedPointer.h"
#include "engine/NUISystem_CManager.h"
template std::_Rb_tree<CCharString, std::pair<const CCharString, CCountedPointer<NUISystem::CComponent> >, std::_Select1st<std::pair<const CCharString, CCountedPointer<NUISystem::CComponent> > >, std::less<CCharString>, std::allocator<std::pair<const CCharString, CCountedPointer<NUISystem::CComponent> > > >::~_Rb_tree();
