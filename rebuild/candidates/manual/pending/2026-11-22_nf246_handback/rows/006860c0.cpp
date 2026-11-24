// std::list<const CThingAICreature*>::list(const allocator&) -- STLport list constructor: allocate the sentinel node and link it to itself
// Leaf: ??0?$list@PBVCThingAICreature@@V?$allocator@PBVCThingAICreature@@@std@@@std@@QAE@ABV?$allocator@PBVCThingAICreature@@@1@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006860C0, 32 bytes, thiscall `ret 4` (the allocator by reference, unused).
//   Callees: malloc 0x00BFEA0E (the 0xC-byte sentinel node).
// Name fix (NF246, 2026-11-22, the 0x00685BC0 family): was the made-up `void** __fastcall DoubleLinkedList_InitializeEmpty(void**, void*,
//   void*)` (catalog `DoubleLinkedList_InitializeEmpty`). Two sources: (a) both landed callers construct a std::list<const
//   CThingAICreature*> here and bind this spelling (CTCCreatureOpinionOfHero::InformOfDeed 0x00682810, the local of its witness line
//   in reloc_bindings, and CAIStateGroup_OpinionReactionRoot::MarriageCommentIsRunnable 0x00986510 since NF246), and both compile the
//   shared-header copy of this constructor equal to these bytes (pk.py); (b) the COMDAT run: it is directly followed by that list's
//   ~list() thunk 0x006860E0 (`jmp 0x00685BC0`, ~_List_base), the list-constructor position of 0x004046F0 / 0x00404710 and
//   0x004AC060 / 0x004AC080. Its bytes equal a _List_base(const allocator&) body (the base constructor inlined), so only the callers'
//   spelling and the run position tell the two apart: descriptive-by-library (STLport 4.6 _list.h `explicit list(const allocator_type&
//   __a = allocator_type()) : _List_base<_Tp, _Alloc>(__a) {}`; the PDBs carry a Dinkumware list).
// Shape: _M_node(allocator, 0) (the proxy's zero store), allocate(1) = malloc(0xC), self-link, publish; returns this.
#include "stlport/fable_containers.h"
class CThingAICreature;
template std::list<const CThingAICreature*, std::allocator<const CThingAICreature*> >::list(const std::allocator<const CThingAICreature*>&);
