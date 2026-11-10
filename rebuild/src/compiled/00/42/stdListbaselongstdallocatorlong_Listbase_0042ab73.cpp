// std::_List_base<long, std::allocator<long> >::~_List_base() -- STLport list base destructor: clear the nodes (out-of-line _List_base::clear @ 0x0042A168) then free the sentinel node
// Leaf: ??1?$_List_base@JV?$allocator@J@std@@@std@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0042AB73, 23 bytes (`mov esi,ecx ; call clear ; if (sentinel) free(sentinel)`),
//   size-optimised TU (`pop ecx`).
// Identity (element type): retail does not fold identical COMDATs, so each of the 165 byte-twins of this body is one list<T>
//   instance. This one is list<long>: SExpSpendUndo::~SExpSpendUndo 0x005797FA destroys its four members through it
//   (struct_layouts_egor SExpSpendUndo: list<long> Generals / Strengths / Skills / Wills), and CTCInventoryTrade's
//   destructor 0x0059EC48 calls it for its only list member (struct_layouts_egor CTCInventoryTrade::NotInStock,
//   list<long>).
// Identity (member, Codex name fix 2026-11-10): STLport 4.6's ~list is empty and inline; the clear + deallocate body is
//   ~_List_base's (_list.h), and retail's out-of-line destructor COMDATs of other lists carry that name (the landed
//   ??1?$_List_base@PAVCClass@@.. 0x00A376A0 binding). Earlier landed as ~list.
#pragma optimize("s", on)
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_STLP_LIST_CLEAR_OUTOFLINE
#include "stlport/fable_containers.h"
template std::_List_base<long, std::allocator<long> >::~_List_base();
