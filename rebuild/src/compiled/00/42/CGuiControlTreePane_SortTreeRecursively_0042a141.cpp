// std::_List_base<NUISystem::CActionParam*>::clear() -- STLport list clear: free every node after the sentinel (the element is a
//   pointer: no element destructor), then link the sentinel to itself
// Leaf: ?clear@?$_List_base@PAVCActionParam@NUISystem@@V?$allocator@PAVCActionParam@NUISystem@@@std@@@std@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0042A141, 39 bytes, size-optimised TU; free 0x00BFEA14.
// Name (NF239, 2026-11-22, CODEXP27 L14): the STLport 4.6 _list.c spelling the landed binder 0x0041D207 already binds here (STL
//   instances have no PDB names). Its other caller is ~_List_base<CActionParam*> 0x0042AB1C (the same unit, respelled with it).
//   Was the stand-in ?Clear@List@@QAEXXZ (catalog _global::StdList_Clear_0042a141).
// Model: stlport/fable_containers.h (FABLE_RETAIL_STLP_LIST) and engine/NUISystem_CAction.h; the explicit instantiation emits the
//   member as its own COMDAT (the 0x0042AB01 pattern).
#pragma optimize("s", on)
#define FABLE_RETAIL_STLP_LIST
#include "engine/NUISystem_CAction.h"
template void std::_List_base<NUISystem::CActionParam*, std::allocator<NUISystem::CActionParam*> >::clear();
