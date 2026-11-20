// std::list<NUISystem::CList*>::pop_front() -- erase(begin()), out of line (STLport 4.6 _list.h)
// Leaf: ?pop_front@?$list@PAVCList@NUISystem@@V?$allocator@PAVCList@NUISystem@@@std@@@std@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0053C7D9, 22 bytes, thiscall: the begin() iterator (head->_M_next) built in a stack slot, the
//   hidden result slot for erase's returned iterator, `call 0x0053C7B9` (list::erase(iterator), out of line), `ret`.
// Identity (two sources): the STLport 4.6 body `void pop_front() { erase(begin()); }` (stlport/fable_containers.h); its callers are
//   NUISystem::CList::RemoveChild 0x0053B321 / CTextSlider::ProcessEvent 0x00549440 (two calls) / CScrollableList 0x00555CF0, all on
//   CList::GPreviousSelected, a std::list<NUISystem::CList*>; the three landed callers already bind this spelling (shared headers).
//   Descriptive-by-library: STLport has no PDB copy (both PDBs are Dinkumware builds); the instantiation is the one the callers prove.
// Was: a free __fastcall pop_front over local Node / ListIt / MyList models (rebuild/src/compiled/00/53/listNUISystemCList_popfront_0053c7d9.cpp).
// Callee: list<NUISystem::CList*>::erase(iterator) 0x0053C7B9 (kept out of line: FABLE_RETAIL_STLP_LIST_ERASE_NOINLINE, as retail calls it).
// Form: an explicit instantiation of the one member over the shared STLport list.
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_STLP_LIST_ERASE_NOINLINE
#include "stlport/fable_containers.h"
namespace NUISystem { class CList; }
template void std::list<NUISystem::CList*>::pop_front();
