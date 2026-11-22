// std::list<NUISystem::CActionParam*>::_M_create_node(CActionParam* const&) -- STLport list node factory: allocate one 12-byte node
//   (malloc) and copy-construct the element pointer at +0x8 in place (the placement new's null test on the element address)
// Leaf: ?_M_create_node@?$list@PAVCActionParam@NUISystem@@V?$allocator@PAVCActionParam@NUISystem@@@std@@@std@@IAEPAU?$_List_node@PAVCActionParam@NUISystem@@@2@ABQAVCActionParam@NUISystem@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00429FBE, 26 bytes, `ret 4`, size-optimised TU (`push 0Ch ; call malloc ; pop ecx`).
// Name (NF239, 2026-11-22, CODEXP27 L13): the STLport 4.6 _list.h spelling the shared header produces (STL instances have no PDB
//   names, and Ego_r / FableWin carry a Dinkumware list: EXTRA_RULES 2026-11-20 "descriptive-by-library"). T = NUISystem::CActionParam*:
//   its only caller is list<CActionParam*>::insert(iterator, const T&) 0x0042AADC (landed binder 0x0042B646 push_back binds that insert
//   under the STLport spelling), the same run as _List_base<CActionParam*> 0x0042AB01 / clear 0x0042A141 / ~_List_base 0x0042AB1C.
//   Was the stand-in ?Buynode@CRetailList_node0xc_dword@@QAEPAUSNode@1@ABJ@Z (catalog CRetailList_node0xc_dword::Buynode), whose
//   caller row bound ?LinkedList_AllocateNode_12bytes@@YGPAULinkedListNode12@@K@Z here.
// Model: stlport/fable_containers.h (FABLE_RETAIL_STLP_LIST) and engine/NUISystem_CAction.h, the 0x0042AB01 pattern: the explicit
//   instantiation emits the member as its own COMDAT.
#pragma optimize("s", on)
#define FABLE_RETAIL_STLP_LIST
#include "engine/NUISystem_CAction.h"
template std::_List_node<NUISystem::CActionParam*>* std::list<NUISystem::CActionParam*, std::allocator<NUISystem::CActionParam*> >::_M_create_node(NUISystem::CActionParam* const&);
