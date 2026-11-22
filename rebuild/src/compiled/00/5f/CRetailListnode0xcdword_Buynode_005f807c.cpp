// std::list<EHeroAbility>::_M_create_node(const EHeroAbility&) -- STLport list node factory: allocate one 12-byte node (malloc) and
//   copy-construct the ability value at +0x8 in place (the placement new's null test on the element address)
// Leaf: ?_M_create_node@?$list@W4EHeroAbility@@V?$allocator@W4EHeroAbility@@@std@@@std@@IAEPAU?$_List_node@W4EHeroAbility@@@2@ABW4EHeroAbility@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x005F807C, 26 bytes, `ret 4`, size-optimised TU.
// Name (NF239, 2026-11-22, CODEXP27 L40): the STLport 4.6 _list.h spelling the shared header produces (STL instances have no PDB
//   names; the PDBs carry a Dinkumware list). T = EHeroAbility: its only caller is list<EHeroAbility>::insert(iterator, const T&)
//   0x005F88C8 (the landed binder 0x005F1B10 CTCInventoryAbilities::ConstructPCMagicList binds that insert under the STLport spelling).
//   Was the stand-in ?Buynode@CRetailList_node0xc_dword@@QAEPAUSNode@1@ABJ@Z (catalog CRetailList_node0xc_dword::Buynode), whose caller
//   row bound ?AllocateInsertNode_005f88c8@@YGPAUInsertNode_005f88c8@@K@Z here.
// Model: stlport/fable_containers.h (FABLE_RETAIL_STLP_LIST) and engine/EHeroAbility.h; the explicit instantiation emits the member as
//   its own COMDAT (the 0x0042AB01 / 0x00429FBE pattern).
#pragma optimize("s", on)
#define FABLE_RETAIL_STLP_LIST
#include "stlport/fable_containers.h"
#include "engine/EHeroAbility.h"
template std::_List_node<EHeroAbility>* std::list<EHeroAbility, std::allocator<EHeroAbility> >::_M_create_node(const EHeroAbility&);
