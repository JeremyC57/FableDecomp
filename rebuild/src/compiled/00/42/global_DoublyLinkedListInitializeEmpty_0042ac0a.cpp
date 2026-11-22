// std::_List_base<NUISystem::CObserver*, std::allocator<NUISystem::CObserver*> >::_List_base(const allocator&) -- STLport list base constructor: allocate the sentinel node and link it to itself (an empty list<CObserver*>)
// Leaf: ??0?$_List_base@PAVCObserver@NUISystem@@V?$allocator@PAVCObserver@NUISystem@@@std@@@std@@QAE@ABV?$allocator@PAVCObserver@NUISystem@@@1@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0042AC0A, 27 bytes, thiscall, `ret 4`, size-optimised TU (`and dword ptr [esi],0`, malloc(0xc), next = prev = node).
// Name (NF229, 2026-11-22, q12615): the STLport spelling the header produces (STL instances have no PDB names: EXTRA_RULES 2026-11-20
//   "descriptive-by-library"). T = NUISystem::CObserver*: its landed binder NUISystem::CObservable::CObservable 0x0042BE7B (nf228; Observers
//   and its second list, FABLE_RETAIL_STLP_LIST_BASE_OUTOFLINE) calls it twice under this spelling.
//   Was the placeholder ?DoublyLinkedList_InitializeEmpty@@YIPAPAXPAPAXPAX1@Z (fastcall, catalog _global::StdList_Ctor_0042ac0a, its HEAD Leaf line DoublyLinkedList_InitializeEmpty; a local sentinel model; the spelling stays on its byte-twin rows 0x0065F1DF ...).
// Model: stlport/fable_containers.h (STLport 4.6 _List_base, FABLE_RETAIL_STLP_LIST) and engine/NUISystem_CObservable.h for the element
//   type; the explicit instantiation emits the member as its own COMDAT (the nf228 0x0042AB01 pattern).
#pragma optimize("s", on)
#define FABLE_RETAIL_STLP_LIST
#include "engine/NUISystem_CObservable.h"
template std::_List_base<NUISystem::CObserver*, std::allocator<NUISystem::CObserver*> >::_List_base(const std::allocator<NUISystem::CObserver*>&);
