// std::_Rb_tree<CCharString, pair<const CCharString, CCountedPointer<NUISystem::CComponent> >, ...>::_Rb_tree(const less&, const allocator&) -- STLport 4.6 tree constructor of NUISystem::CManager's map<CCharString, CCountedPointer<CComponent> > (BastardChildren): header node allocated (_Rb_tree_base), node count 0, then _M_empty_initialize
// Leaf: ??0?$_Rb_tree@VCCharString@@U?$pair@$$CBVCCharString@@V?$CCountedPointer@VCComponent@NUISystem@@@@@std@@U?$_Select1st@U?$pair@$$CBVCCharString@@V?$CCountedPointer@VCComponent@NUISystem@@@@@std@@@3@U?$less@VCCharString@@@3@V?$allocator@U?$pair@$$CBVCCharString@@V?$CCountedPointer@VCComponent@NUISystem@@@@@std@@@3@@std@@QAE@ABU?$less@VCCharString@@@1@ABV?$allocator@U?$pair@$$CBVCCharString@@V?$CCountedPointer@VCComponent@NUISystem@@@@@std@@@1@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0042B7EE, 42 bytes, size-optimised TU.
// Name (NF228, 2026-11-22, q12595): the STLport spelling the header produces (STL instances have no PDB names: EXTRA_RULES 2026-11-20
//   "descriptive-by-library"); its only landed binder, map() 0x0042D28B, calls it under this spelling (its pick-any copy of this member equals this address).
//   Was the placeholder ?CTexture_InitEmpty_28bytes@NodeInitOwner_0042b7ee@@... (catalog CTexture_InitEmpty_28bytes, a row-local stand-in struct).
// Model: engine/NUISystem_CManager.h (the member's map type) and stlport/fable_map.h (the default-path inline member); the explicit
//   instantiation emits the member as its own COMDAT (the 0x008353B0 pattern). Defines and pragma as 0x0042D28B (the same retail TU).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA
#define FABLE_RETAIL_STLP_MAP
#pragma optimize("s", on)
#include "engine/CCountedPointer.h"
#include "engine/NUISystem_CManager.h"
template std::_Rb_tree<CCharString, std::pair<const CCharString, CCountedPointer<NUISystem::CComponent> >, std::_Select1st<std::pair<const CCharString, CCountedPointer<NUISystem::CComponent> > >, std::less<CCharString>, std::allocator<std::pair<const CCharString, CCountedPointer<NUISystem::CComponent> > > >::_Rb_tree(const std::less<CCharString>&, const std::allocator<std::pair<const CCharString, CCountedPointer<NUISystem::CComponent> > >&);
