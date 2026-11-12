// std::_S_merge<const CThingCreatureBase*, allocator<const CThingCreatureBase*>, CThingCompare_Nearest>(list& __that, list& __x, CThingCompare_Nearest __comp) -- STLport list merge: splice the nodes of the sorted list __x into the sorted __that in comparator order (nearest first), then append what remains
// Leaf: ??$_S_merge@PBVCThingCreatureBase@@V?$allocator@PBVCThingCreatureBase@@@std@@VCThingCompare_Nearest@@@std@@YIXAAV?$list@PBVCThingCreatureBase@@V?$allocator@PBVCThingCreatureBase@@@std@@@0@0VCThingCompare_Nearest@@@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006F04C0, length 139 bytes (`ret 0Ch`: the 12-byte comparator by value; the two lists
//   in ecx/edx -- the STL algorithm TUs are /Gr, free templates are __fastcall). Callers: 0x006F3B01 and 0x006F3B77, the merge steps
//   of the landed std::_S_sort<const CThingCreatureBase*, ..., CThingCompare_Nearest> 0x006F3A30.
// Name-fix 2026-11-12 (was the stand-in free _S_merge(SMarkerList&, SMarkerList&, CThingCompare_Nearest) over CThingMarker* with local
//   SListNode / SMarkerList / CThingCompare_Nearest models; catalog _global::_S_merge, manifest placeholder CBinaryTree_Alt::Rebalance):
//   STLport 4.6 _list.c `_S_merge(list<_Tp,_Alloc>& __that, list<_Tp,_Alloc>& __x, _StrictWeakOrdering __comp)`, instantiated with the
//   element / comparator types of its only caller _S_sort (Ego_r.pdb 0x0071A120 + FableWin.pdb 0x02433550
//   ??$sort@VCThingCompare_Nearest@@@?$list@PBVCThingCreatureBase@@...), which binds this address under this (constructed) symbol.
//   Body: stlport/fable_containers.h (FABLE_RETAIL_STLP_LIST_SORT), explicitly instantiated; the comparator's operator() 0x006EF290
//   is out of line (FABLE_RETAIL_CTHINGCOMPARE_NEAREST_OUTOFLINE), as in the _S_sort row.
#define FABLE_RETAIL_STLP_LIST_SORT
#define FABLE_RETAIL_CTHINGCOMPARE_NEAREST_OUTOFLINE
#include "stlport/fable_containers.h"
#include "engine/CThingCompare_Nearest.h"
class CThingCreatureBase;
template void FABLE_STLP_FASTCALL std::_S_merge<const CThingCreatureBase*,std::allocator<const CThingCreatureBase*>,CThingCompare_Nearest>(std::list<const CThingCreatureBase*,std::allocator<const CThingCreatureBase*> >&,std::list<const CThingCreatureBase*,std::allocator<const CThingCreatureBase*> >&,CThingCompare_Nearest);
