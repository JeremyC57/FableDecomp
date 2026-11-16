// std::list::_M_insert_dispatch<const_iterator>(iterator pos, const_iterator first, const_iterator last, const __false_type&) -- list<NUISystem::CComponent*>: insert a copy of each element of [first, last) before pos
// Leaf: ??$_M_insert_dispatch@U?$_List_iterator@PAVCComponent@NUISystem@@U?$_Const_traits@PAVCComponent@NUISystem@@@std@@@std@@@?$list@PAVCComponent@NUISystem@@V?$allocator@PAVCComponent@NUISystem@@@std@@@std@@QAEXU?$_List_iterator@PAVCComponent@NUISystem@@U?$_Nonconst_traits@PAVCComponent@NUISystem@@@std@@@1@U?$_List_iterator@PAVCComponent@NUISystem@@U?$_Const_traits@PAVCComponent@NUISystem@@@std@@@1@1ABU__false_type@1@@Z
// Retail: Fable.exe @ 0x0042B4C3, 48 bytes, `ret 10h` (pos, first, last and the __false_type tag); 12 callers (0x0042B95A,
// 0x005325F6, 0x0053268D, ...). STLport's range-insert loop: while first != last, insert(pos, *first) (0x0042AB8A, which
// returns an iterator through a hidden pointer -- the dead `first` slot is reused for it) and ++first (node +0 next,
// value at node +8). Element type (sweep2): NUISystem::CComponent* -- callers are list<CComponent*>::operator= 0x00535800 and the range insert 0x0042B945 under CDeletion's list copy ctor (Ego_r CDeletion::AssociatedParents list<CComponent*>); was the placeholder NUISystem::CComponent*.
// Layout: STLport list (one sentinel-node pointer), node {next +0, prev +4, data +8}; iterators are one node pointer
// with a user copy constructor (the pos copy is built in its argument slot: `push ecx ; mov eax,esp ; mov [eax],ecx`).
// Opt: size TU with frame pointers (`optimize("y", off)`: push ebp / mov ebp,esp; unaligned entry).
// NF42 (round 5): the local list / iterator / node models (a one-parameter list, mangling insert's caller-visible spelling without the
// allocator) replaced by the shared stlport/fable_containers.h (FABLE_RETAIL_STLP_LIST_INSERT_OUTOFLINE: insert(iterator, const T&)
// visible but called, 0x0042AB8A), so the insert call binds STLport's full ?insert@?$list@..V?$allocator@..@std@@@std@@.. spelling.
#pragma optimize("s", on)
#pragma optimize("y", off)
#define FABLE_RETAIL_STLP_LIST_INSERT_OUTOFLINE
#include "stlport/fable_containers.h"
namespace NUISystem { class CComponent; }
template void std::list<NUISystem::CComponent*>::_M_insert_dispatch<std::list<NUISystem::CComponent*>::const_iterator>(
    std::list<NUISystem::CComponent*>::iterator, std::list<NUISystem::CComponent*>::const_iterator,
    std::list<NUISystem::CComponent*>::const_iterator, const std::__false_type&);
