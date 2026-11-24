// std::_Rb_tree_base<std::pair<const CCharString, CCountedPointer<NUISystem::CComponent> >, std::allocator<std::pair<const CCharString, CCountedPointer<NUISystem::CComponent> > > >::_Rb_tree_base(const allocator&) -- STLport 4.6 tree base constructor of NUISystem::CManager's map<CCharString, CCountedPointer<CComponent> > (BastardChildren): clear the header-node pointer, then allocate the header node (malloc(0x1c))
// Leaf: ??0?$_Rb_tree_base@U?$pair@$$CBVCCharString@@V?$CCountedPointer@VCComponent@NUISystem@@@@@std@@V?$allocator@U?$pair@$$CBVCCharString@@V?$CCountedPointer@VCComponent@NUISystem@@@@@std@@@2@@std@@QAE@ABV?$allocator@U?$pair@$$CBVCCharString@@V?$CCountedPointer@VCComponent@NUISystem@@@@@std@@@1@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0042AD10, 22 bytes, thiscall, `ret 4`, size-optimised TU (`and dword ptr [esi],0`, malloc(0x1c)).
// Name (NF229, 2026-11-22, q12613): the STLport spelling the header produces (STL instances have no PDB names: EXTRA_RULES 2026-11-20
//   "descriptive-by-library"). Its landed binder, the _Rb_tree(const less&, const allocator&) constructor 0x0042B7EE (nf228), binds it under
//   this spelling. Was the placeholder ?Allocator_Alloc_28bytes@@YIPAPAXPAPAXPAXPBX@Z (fastcall, catalog Allocator_Alloc_28bytes; no model).
// Model: engine/NUISystem_CManager.h (the member's map type) and stlport/fable_map.h (STLport _tree.h _Rb_tree_base); the explicit
//   instantiation emits the member as its own COMDAT (the nf228 0x0042B76C pattern). Defines and pragma as 0x0042B7EE (same retail TU).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA
#define FABLE_RETAIL_STLP_MAP
#pragma optimize("s", on)
#include "engine/CCountedPointer.h"
#include "engine/NUISystem_CManager.h"
template std::_Rb_tree_base<std::pair<const CCharString, CCountedPointer<NUISystem::CComponent> >, std::allocator<std::pair<const CCharString, CCountedPointer<NUISystem::CComponent> > > >::_Rb_tree_base(const std::allocator<std::pair<const CCharString, CCountedPointer<NUISystem::CComponent> > >&);
