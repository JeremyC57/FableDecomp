// std::_List_base<NUISystem::CActionParam*>::~_List_base() -- STLport list base destructor: clear the nodes (0x0042A141, out of
//   line) then free the sentinel node
// Leaf: ??1?$_List_base@PAVCActionParam@NUISystem@@V?$allocator@PAVCActionParam@NUISystem@@@std@@@std@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0042AB1C, 23 bytes, size-optimised TU (`pop ecx` after the free).
// Name (NF239, 2026-11-22, CODEXP27 L16): the STLport 4.6 _list.h spelling the landed binders 0x0041E641 (~CManager), 0x0054DED0 and
//   0x005954AD already bind here (STL instances have no PDB names). Was the stand-in ??1list_T_0042ab1c@std@@QAE@XZ (catalog
//   std::list<T_0042ab1c>::~list); its clear callee bound ?clear@list_T_0042ab1c@std@@QAEXXZ at 0x0042A141, now the STLport clear.
// Model: stlport/fable_containers.h (FABLE_RETAIL_STLP_LIST; FABLE_RETAIL_STLP_LIST_CLEAR_OUTOFLINE: clear declared only, called out
//   of line as retail does) and engine/NUISystem_CAction.h; the explicit instantiation emits the destructor as its own COMDAT.
#pragma optimize("s", on)
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_STLP_LIST_CLEAR_OUTOFLINE
#include "engine/NUISystem_CAction.h"
template std::_List_base<NUISystem::CActionParam*, std::allocator<NUISystem::CActionParam*> >::~_List_base();
