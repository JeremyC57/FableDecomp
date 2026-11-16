// std::list<NUISystem::CComponent*>::push_back(NUISystem::CComponent* const& x) -- STLport 4.6 _list.h `insert(end(), x)` (0x0042AB8A)
// Leaf: ?push_back@?$list@PAVCComponent@NUISystem@@V?$allocator@PAVCComponent@NUISystem@@@std@@@std@@QAEXABQAVCComponent@NUISystem@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0042B6B8, 26 bytes, size TU with a frame pointer; the end() iterator (the sentinel, the list's only
// word) copied into its argument slot, the hidden result in the dead parameter slot.
// Identity (NF42, round 5): the body passes the sentinel (end()), so this is push_back, as the landed caller 0x0059DE04 names it (its
// one-parameter list spelling lacks the allocator); STLport 4.6.2, descriptive-by-library. Was the placeholder
// `StdList::Std_List_PushFront` calling `StdList::Insert`; now an explicit instantiation over stlport/fable_containers.h
// (FABLE_RETAIL_STLP_LIST_INSERT_OUTOFLINE + FABLE_RETAIL_STLP_LIST_PUSHBACK_NOINLINE), so insert binds the list<CComponent*> claim.
#pragma optimize("s", on)
#pragma optimize("y", off)
#define FABLE_RETAIL_STLP_LIST_INSERT_OUTOFLINE
#define FABLE_RETAIL_STLP_LIST_PUSHBACK_NOINLINE
#include "stlport/fable_containers.h"
namespace NUISystem { class CComponent; }
template void std::list<NUISystem::CComponent*>::push_back(NUISystem::CComponent* const&);
