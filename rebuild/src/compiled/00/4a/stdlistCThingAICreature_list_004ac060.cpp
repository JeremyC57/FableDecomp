// std::list<CThingAICreature*>::list(const allocator&) -- STLport list constructor: allocate the sentinel node and link it to itself
// Leaf: ??0?$list@PAVCThingAICreature@@V?$allocator@PAVCThingAICreature@@@std@@@std@@QAE@ABV?$allocator@PAVCThingAICreature@@@1@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004AC060, 32 bytes, thiscall `ret 4` (the allocator by reference, unused).
//   Callees: malloc 0x00BFEA0E (the 0xC-byte sentinel node).
// Name fix (NF246, 2026-11-22, the 0x004AB4F0 family): was the made-up `void** __fastcall ListNode_AllocateSelfLinked_RegionLoad(void**,
//   void*, void*)` (catalog `ListNode_AllocateSelfLinked_RegionLoad`). Two sources: (a) its only landed caller, CWorld::UpdateRegionLoad
//   0x004A3740 (stlport/fable_containers.h), constructs a std::list<CThingAICreature*> here and binds this spelling (the witness of its
//   reloc_bindings line), and the shared-header copy of this constructor compiled by 0x005EB590 equals these bytes (pk.py); (b) the COMDAT
//   run: it is directly followed by that list's ~list() thunk 0x004AC080 (`jmp 0x004AB4F0`, ~_List_base), the list-constructor position
//   of 0x004046F0 / 0x00404710 and 0x006860C0 / 0x006860E0. Its bytes equal a _List_base(const allocator&) body (the base constructor
//   inlined), so only the caller's spelling and the run position tell the two apart: descriptive-by-library (STLport 4.6 _list.h; the
//   PDBs carry a Dinkumware list).
// Shape: _M_node(allocator, 0) (the proxy's zero store), allocate(1) = malloc(0xC), self-link, publish; returns this.
#include "stlport/fable_containers.h"
class CThingAICreature;
template std::list<CThingAICreature*, std::allocator<CThingAICreature*> >::list(const std::allocator<CThingAICreature*>&);
