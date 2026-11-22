// std::_List_base<NUISystem::CActionParam*, std::allocator<NUISystem::CActionParam*> >::_List_base(const allocator&) -- STLport list base constructor: allocate the sentinel node and link it to itself (an empty list<CActionParam*>)
// Leaf: ??0?$_List_base@PAVCActionParam@NUISystem@@V?$allocator@PAVCActionParam@NUISystem@@@std@@@std@@QAE@ABV?$allocator@PAVCActionParam@NUISystem@@@1@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0042AB01, 27 bytes, `ret 4`, size-optimised TU (`and dword ptr [esi],0`, malloc(0xc), next = prev = node).
// Name (NF228, 2026-11-22, q12596): the STLport spelling the header produces (STL instances have no PDB names: EXTRA_RULES 2026-11-20
//   "descriptive-by-library"). T = NUISystem::CActionParam*: the landed binders NUISystem::CManager::CManager 0x0041E3F6 and
//   NUISystem::CAction::CAction 0x0042BE50 (UIActionParams, Ego_r.pdb list<CActionParam*>) call it under this spelling from the inline
//   list(const allocator&) (FABLE_RETAIL_STLP_LIST_BASE_OUTOFLINE). 0x005953E2 binds the list<CActionParam*>(const allocator&) spelling
//   at the same address: retail folded that inline list constructor into this body (folded alias, split queued at nf227).
//   Was the placeholder ?Std_DoubleLinkedList_CreateNode@@YIPAPAUDoubleListNode@@PAPAU1@PAX1@Z (fastcall, catalog
//   Std_DoubleLinkedList_CreateNode); the HEAD source also defined two unreferenced placeholder copies (Copy_MemoryStepped,
//   CActiveFile_CopyRange), dropped.
// Model: stlport/fable_containers.h (STLport 4.6 _List_base, FABLE_RETAIL_STLP_LIST) and engine/NUISystem_CAction.h for the element type;
//   the explicit instantiation emits the member as its own COMDAT (the _List_base<long> 0x0042AB58 pattern).
#pragma optimize("s", on)
#define FABLE_RETAIL_STLP_LIST
#include "engine/NUISystem_CAction.h"
template std::_List_base<NUISystem::CActionParam*, std::allocator<NUISystem::CActionParam*> >::_List_base(const std::allocator<NUISystem::CActionParam*>&);
