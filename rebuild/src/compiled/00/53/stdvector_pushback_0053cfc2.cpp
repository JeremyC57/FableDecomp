// std::vector<CUIStateMap>::push_back(const CUIStateMap& x) -- STLport vector push_back of NUISystem::CList::ScrollingChildrenStates (CUIStateMap = map<unsigned long, NUISystem::CUIState>): copy-construct the map at the end when there is room, else grow through _M_insert_overflow
// Leaf: ?push_back@?$vector@V?$map@KVCUIState@NUISystem@@U?$less@K@std@@V?$allocator@U?$pair@$$CBKVCUIState@NUISystem@@@std@@@4@@std@@V?$allocator@V?$map@KVCUIState@NUISystem@@U?$less@K@std@@V?$allocator@U?$pair@$$CBKVCUIState@NUISystem@@@std@@@4@@std@@@2@@std@@QAEXABV?$map@KVCUIState@NUISystem@@U?$less@K@std@@V?$allocator@U?$pair@$$CBKVCUIState@NUISystem@@@std@@@4@@2@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0053CFC2, 56 bytes, `ret 4` (thiscall on the vector, &x); size-optimised frame-pointer
//   TU (`push ebp ; mov ebp,esp`, unaligned entry: `optimize("y", off)` + `("s", on)`, as the landed CList rows).
// Callees: map<unsigned long, CUIState> copy constructor 0x0053CB9B (manifest "std::list_construct_from_list": a 0x38-byte
//   header node from malloc, then the tree copy), the vector's _M_insert_overflow(pos, x, const __false_type&, 1, true)
//   0x0053CEF6 (manifest "CActiveFile::Read_CArrayElements").
// Callers: NUISystem::CList 0x0053B964 (Initialise 0x0053B91E), 0x0053BEF4 (AddScrollingChildDynamically 0x0053BEAC,
//   landed), 0x0053C2C1 (AddScrollingChildDynamicallyAndSetOffsets 0x0053BF20), 0x0053C764 (DoRecomputeOffsets 0x0053C332).
// Label correction: catalogued as "UIScrollList_AddChild" (the landed 0x0053BEAC declares it as a CScrollLayout::AddChild
//   at +0x17c); +0x17c is CList::ScrollingChildrenStates (Ego_r.pdb +0x1b4, vector<map<unsigned long, CUIState> >; the
//   retail vectors are 12 bytes) and this is its push_back.
// Layout: STLport vector { start, finish, end of storage }; the map element is 12 bytes { header, count, comparator }.
// Shape: STLport `if (_M_finish != _M_end_of_storage) { _Construct(_M_finish, x); ++_M_finish; } else
//   _M_insert_overflow(_M_finish, x, __false_type(), 1UL, true);` -- the placement copy is null tested, the tag temporary
//   sits in the high byte of the `x` argument slot (`lea eax,[ebp+0Bh]`). An explicit instantiation emits only push_back.
// Name fix (NF56, round 7, queue q10461): Ego_r.pdb 0x005B6299 + FableWin.pdb 0x021A3350 spell this vector<map<unsigned long,
//   NUISystem::CUIState> >::push_back (CList::ScrollingChildrenStates, engine/NUISystem_CList.h). The local std::vector / CUIStateMap
//   models are replaced by the shared STLport containers (stlport/fable_containers.h, stlport/fable_map.h with the map copy constructor
//   out of line, FABLE_RETAIL_STLP_MAP_COPY_OUTOFLINE: the COMDAT 0x0053CB9B; the overflow 0x0053CEF6 declared only,
//   FABLE_RETAIL_STLP_VECTOR_OVERFLOW_DECL_ONLY) and engine/NUISystem_CUIState.h.
#pragma optimize("y", off)
#pragma optimize("s", on)
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_STLP_MAP_COPY_OUTOFLINE
#define FABLE_RETAIL_STLP_VECTOR_OVERFLOW_DECL_ONLY
#include "stlport/fable_containers.h"
#include "stlport/fable_map.h"
#include "engine/NUISystem_CUIState.h"

template void std::vector<std::map<unsigned long, NUISystem::CUIState> >::push_back(const std::map<unsigned long, NUISystem::CUIState>& x);
