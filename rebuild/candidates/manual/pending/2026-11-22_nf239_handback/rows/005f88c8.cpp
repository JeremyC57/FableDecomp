// std::list<EHeroAbility>::insert(iterator position, const EHeroAbility& x) -- STLport list insert: build the node (_M_create_node
//   0x005F807C, out of line), link it before position, return its iterator through the hidden result pointer
// Leaf: ?insert@?$list@W4EHeroAbility@@V?$allocator@W4EHeroAbility@@@std@@@std@@QAE?AU?$_List_iterator@W4EHeroAbility@@U?$_Nonconst_traits@W4EHeroAbility@@@std@@@2@U32@ABW4EHeroAbility@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x005F88C8, 37 bytes, size-optimised TU.
// Name (NF239, 2026-11-22, CODEXP27 L40): the STLport spelling the landed binder 0x005F1B10 already binds here; was the stand-in
//   ?LinkedList_InsertNodeBefore@@YGPAPAUInsertNode_005f88c8@@PAPAU1@PAU1@K@Z (catalog LinkedList_InsertNodeBefore). Its callee 0x005F807C
//   is list<EHeroAbility>::_M_create_node (the same unit). 0x005F8F50 still binds ?InsertBefore@ListWrapper_005f8f50@@ here (NOTES).
// Model: stlport/fable_containers.h (FABLE_RETAIL_STLP_LIST; FABLE_RETAIL_STLP_LIST_CREATE_NODE_NOINLINE keeps _M_create_node out of
//   line, as retail calls it) and engine/EHeroAbility.h; the explicit instantiation emits insert as its own COMDAT.
#pragma optimize("s", on)
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_STLP_LIST_CREATE_NODE_NOINLINE
#include "stlport/fable_containers.h"
#include "engine/EHeroAbility.h"
template std::list<EHeroAbility, std::allocator<EHeroAbility> >::iterator std::list<EHeroAbility, std::allocator<EHeroAbility> >::insert(std::list<EHeroAbility, std::allocator<EHeroAbility> >::iterator, const EHeroAbility&);
