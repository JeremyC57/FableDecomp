// std::list<NUISystem::CActionParam*>::push_back(NUISystem::CActionParam* const& x) -- STLport 4.6 _list.h `insert(end(), x)` through the
// out-of-line insert
// Leaf: ?push_back@?$list@PAVCActionParam@NUISystem@@V?$allocator@PAVCActionParam@NUISystem@@@std@@@std@@QAEXABQAVCActionParam@NUISystem@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0042B646, size TU with a frame pointer; the end() iterator (the sentinel, the list's only word)
// copied into its argument slot, the hidden iterator result in the dead parameter slot (twin of 0x0042B687 / 0x0042B6B8).
// Identity (NF104, 2026-11-19, queue q4770): FableWin.pdb ?push_back@?$list@PAVCActionParam@NUISystem@@V?$allocator@...@std@@@std@@QAEX
//   ABQAVCActionParam@NUISystem@@@Z, and 50 of the 51 landed rows that call 0x0042B646 (0x0041DB64, 0x0041DC9D, 0x0041E6D3, 0x00544D20 ...)
//   already bind that symbol; the body passes the sentinel (end()), so this is push_back. STLport 4.6.2, descriptive-by-library.
//   Was the stand-in ListWrapper_0042b646::ListPushFrontWrapper_0042b646 calling InsertBefore; now an explicit instantiation over
//   stlport/fable_containers.h (FABLE_RETAIL_STLP_LIST_INSERT_OUTOFLINE + FABLE_RETAIL_STLP_LIST_PUSHBACK_NOINLINE), the 0x0042B6B8 recipe.
#pragma optimize("s", on)
#pragma optimize("y", off)
#define FABLE_RETAIL_STLP_LIST_INSERT_OUTOFLINE
#define FABLE_RETAIL_STLP_LIST_PUSHBACK_NOINLINE
#include "stlport/fable_containers.h"
namespace NUISystem { class CActionParam; }
template void std::list<NUISystem::CActionParam*>::push_back(NUISystem::CActionParam* const&);
