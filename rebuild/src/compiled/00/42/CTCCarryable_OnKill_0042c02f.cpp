// std::_Rb_tree<CCharString, pair<const CCharString, CCountedPointer<NUISystem::CComponent> >, ...>::clear() -- STLport 4.6 tree clear of NUISystem::CManager's map<CCharString, CCountedPointer<CComponent> > (BastardChildren): when the tree is not empty, erase every node from the root (_M_erase, out of line), then reset the header node (leftmost = rightmost = header, root 0) and the node count
// Leaf: ?clear@?$_Rb_tree@VCCharString@@U?$pair@$$CBVCCharString@@V?$CCountedPointer@VCComponent@NUISystem@@@@@std@@U?$_Select1st@U?$pair@$$CBVCCharString@@V?$CCountedPointer@VCComponent@NUISystem@@@@@std@@@3@U?$less@VCCharString@@@3@V?$allocator@U?$pair@$$CBVCCharString@@V?$CCountedPointer@VCComponent@NUISystem@@@@@std@@@3@@std@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0042C02F, 41 bytes, size-optimised TU (the repeated header-node reloads are STLport's `_M_header._M_data` accesses).
// Name (NF228, 2026-11-22, q12592): the STLport spelling the header produces (STL instances have no PDB names: EXTRA_RULES 2026-11-20
//   "descriptive-by-library"). The landed binders all bind it: ClearBastardChild 0x0041DED2 (FABLE_RETAIL_STLP_TREE_CLEAR_OUTOFLINE), the
//   ~_Rb_tree 0x0042CE63 and ~CManager 0x0041E641 (their pick-any copies of this member equal this address, relocations alike).
//   Was the placeholder ?OnKill@T@@QAEXXZ (catalog CTCCarryable::OnKill, a byte-twin label of family G-len41; a local `struct T` model).
// Model: engine/NUISystem_CManager.h (the member's map type) and stlport/fable_map.h (default-path inline clear); the explicit
//   instantiation emits the member as its own COMDAT (the 0x008353B0 / 0x0042CE63 pattern). Defines and pragma as 0x0042CE63 (same retail TU).
#define FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA
#pragma optimize("s", on)
#include "engine/CCountedPointer.h"
#include "engine/NUISystem_CManager.h"
template void std::_Rb_tree<CCharString, std::pair<const CCharString, CCountedPointer<NUISystem::CComponent> >, std::_Select1st<std::pair<const CCharString, CCountedPointer<NUISystem::CComponent> > >, std::less<CCharString>, std::allocator<std::pair<const CCharString, CCountedPointer<NUISystem::CComponent> > > >::clear();
