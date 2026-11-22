// std::list<NUISystem::CActionParam*>::insert(iterator position, CActionParam* const& x) -- STLport list insert: build the node
//   (_M_create_node 0x00429FBE, out of line), link it before position, return its iterator through the hidden result pointer
// Leaf: ?insert@?$list@PAVCActionParam@NUISystem@@V?$allocator@PAVCActionParam@NUISystem@@@std@@@std@@QAE?AU?$_List_iterator@PAVCActionParam@NUISystem@@U?$_Nonconst_traits@PAVCActionParam@NUISystem@@@std@@@2@U32@ABQAVCActionParam@NUISystem@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0042AADC, 37 bytes, size-optimised TU.
// Name (NF239, 2026-11-22, CODEXP27 L13): the STLport spelling the landed binder 0x0042B646 (list<CActionParam*>::push_back) already
//   binds here; was the stand-in ?LinkedList_InsertBefore@@YGPAPAULinkedListNode12@@PAPAU1@PAU1@K@Z (catalog LinkedList_InsertBefore).
//   Its callee 0x00429FBE is list<CActionParam*>::_M_create_node (the same unit).
// Model: stlport/fable_containers.h (FABLE_RETAIL_STLP_LIST; FABLE_RETAIL_STLP_LIST_CREATE_NODE_NOINLINE keeps _M_create_node out of
//   line, as retail calls it) and engine/NUISystem_CAction.h; the explicit instantiation emits insert as its own COMDAT.
#pragma optimize("s", on)
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_STLP_LIST_CREATE_NODE_NOINLINE
#include "engine/NUISystem_CAction.h"
template std::list<NUISystem::CActionParam*, std::allocator<NUISystem::CActionParam*> >::iterator std::list<NUISystem::CActionParam*, std::allocator<NUISystem::CActionParam*> >::insert(std::list<NUISystem::CActionParam*, std::allocator<NUISystem::CActionParam*> >::iterator, NUISystem::CActionParam* const&);
