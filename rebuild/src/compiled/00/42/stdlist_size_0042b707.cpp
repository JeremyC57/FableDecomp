// std::list<NUISystem::CList*>::size() const -- STLport distance(begin(), end()): walk the ring from the sentinel's next node back to the sentinel,
// counting the nodes
// Leaf: ?size@?$list@PAVCList@NUISystem@@V?$allocator@PAVCList@NUISystem@@@std@@@std@@QBEIXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0042B707, thiscall, plain `ret`; no callees; size TU (unaligned entry).
// NF104 (q4909, 2026-11-19): moved off its `List::Count()` stand-in (catalog label "CBasicString<char>::CalcStringLength", propagated) onto
//   the PDB spelling: FableWin.pdb 0x01B12092 ?size@?$list@PAVCList@NUISystem@@V?$allocator@PAVCList@NUISystem@@@std@@@std@@QBEIXZ (the
//   type: NUISystem::CList::GPreviousSelected 0x013B8A98, engine/NUISystem_CList.h); CTextSlider::ProcessEvent 0x00549440 calls it on that
//   list (`size() > 1`) and binds this symbol (RELOCATION_MATCH). The body is STLport's walk, not Dinkumware's _Mysize read.
// Model: the shared stlport/fable_containers.h list (STLport 4.6 stl/_list.h; opt-ins FABLE_RETAIL_STLP_LIST + _SIZE_DISTANCE: STLport's own
//   `distance(begin(), end())`), as the landed 0x00ADB570 list<CVertexGroupBlock>::size; explicit instantiation emits the member as its own COMDAT.
#pragma optimize("s",on)
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_STLP_LIST_SIZE_DISTANCE
#include "stlport/fable_map.h"
namespace NUISystem { class CList; }

template unsigned int std::list<NUISystem::CList*, std::allocator<NUISystem::CList*> >::size() const;
