// std::_List_base<const CMessageEvent*, std::allocator<const CMessageEvent*> >::clear() -- STLport 4.6 list clear: free every node,
// then link the sentinel to itself (was the placeholder _global::StdList_Clear_00493911 / ?Clear@List@@QAEXXZ).
// Leaf: ?clear@?$_List_base@PBVCMessageEvent@@V?$allocator@PBVCMessageEvent@@@std@@@std@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00493911, thiscall, size TU (#pragma optimize("s")); callee free 0x00BFEA14.
// Naming (agentNF32, 2026-11-12): DESCRIPTIVE-BY-LIBRARY. Retail is STLport 4.6; Ego_r.pdb / FableWin.pdb are Dinkumware builds, so
//   no PDB carries this spelling. Evidence: the body is the STLport _List_base::clear of stlport/fable_containers.h (pointer element,
//   no destructor call); the landed _List_base destructor 0x00493BDD calls this address and binds this exact symbol
//   (rebuild/oracles/reloc_bindings.tsv). T = const CMessageEvent* rests on that caller; no other template arguments are invented.
#pragma optimize("s", on)
#define FABLE_RETAIL_STLP_LIST
#include "stlport/fable_containers.h"
class CMessageEvent;
template void std::_List_base<const CMessageEvent*, std::allocator<const CMessageEvent*> >::clear();
