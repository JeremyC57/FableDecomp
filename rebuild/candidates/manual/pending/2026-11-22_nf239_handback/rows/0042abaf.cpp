// std::_List_base<NUISystem::CComponent*>::_List_base(const allocator&) -- STLport list base constructor: allocate the sentinel node
//   and link it to itself (an empty list<CComponent*>)
// Leaf: ??0?$_List_base@PAVCComponent@NUISystem@@V?$allocator@PAVCComponent@NUISystem@@@std@@@std@@QAE@ABV?$allocator@PAVCComponent@NUISystem@@@1@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0042ABAF, 27 bytes, `ret 4` (the allocator reference), returns this; size-optimised TU
//   (`and [esi],0`, malloc(0xc), next = prev = node).
// Name (NF239, 2026-11-22, CODEXP27 L17): the base-class constructor, not list's own. Two sources: retail 0x0042B6A6 is the
//   size-optimised list<CComponent*>::list(const allocator&) COMDAT, `push [esp+8] ; mov esi,ecx ; call 0x0042ABAF ; mov eax,esi ;
//   ret 4` (STLport 4.6 _list.h `explicit list(const allocator_type& __a) : _List_base<_Tp,_Alloc>(__a) {}`), so 0x0042ABAF is
//   _List_base's constructor (STLport 4.6 _list.h: the sentinel allocation lives in _List_base); the landed binders 0x0042BF06,
//   0x0058AF10, 0x00598A1C, 0x005B01FE, 0x005B0AAC, 0x005B690B already bind this spelling. (Ego_r / FableWin carry a Dinkumware list
//   with no _List_base: no PDB arbitrates STLport layering.) Was ??0?$list@PAVCComponent@..@QAE@ABV?$allocator@..@1@@Z from a
//   row-local list model; 10 landed binders still spell that (the list<CComponent*> ctor/dtor family pass, NOTES).
// Model: stlport/fable_containers.h (the 0x0042AB01 pattern): the explicit instantiation emits the member as its own COMDAT.
#pragma optimize("s", on)
#include "stlport/fable_containers.h"
namespace NUISystem { class CComponent; }
template std::_List_base<NUISystem::CComponent*, std::allocator<NUISystem::CComponent*> >::_List_base(const std::allocator<NUISystem::CComponent*>&);
