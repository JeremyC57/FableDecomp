// std::vector<NUISystem::CComponent*>::erase(iterator first, iterator last) -- STLport 4.6 range erase: copy the tail [last, finish)
//   down to first through the out-of-line __copy_trivial, set the new finish, return first (the pointer elements need no destroy)
// Leaf: ?erase@?$vector@PAVCComponent@NUISystem@@V?$allocator@PAVCComponent@NUISystem@@@std@@@std@@QAEPAPAVCComponent@NUISystem@@PAPAV34@0@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00485AA7, 30 bytes, thiscall, `ret 8` (size-optimised TU):
//   push esi ; push [esp+8] (first) ; mov esi,ecx ; mov edx,[esi+4] (finish) ; mov ecx,[esp+10h] (last) ; call 0x00405BA0 ;
//   mov [esi+4],eax ; mov eax,[esp+8] ; pop esi ; ret 8
// Callee: std::__copy_trivial 0x00405BA0 (Ego_r / FableWin ?__copy_trivial@std@@YIPAXPBX0PAX@Z, __fastcall; the header's
//   FABLE_RETAIL_STLP_COPY_TRIVIAL_OUTOFLINE declaration; was the made-up ?ListBoxDataHelper@@YIKKKK@Z here).
// Name fix (NF202, 2026-11-20, q12271): was a July model CGuiControlListBox::SelectEntryFromData (no Leaf). STLport spelling
//   (descriptive-by-library: no PDB holds this instance); the catalog reads std::vector<NUISystem::CComponent*>::erase, and the landed
//   callers 0x00587847 (CTCInventory::ClosePCInventory) and 0x00485D25 (vector<CComponent*>::clear) bind this spelling.
//   New pick-any COMDATs from the header (_Destroy / __destroy / __destroy_aux / __copy_ptrs on CComponent**, __true_type ctor) have
//   no table line and no owner row.
#include "rebuild_abi.h"
#define FABLE_RETAIL_STLP_COPY_TRIVIAL_OUTOFLINE
#include "stlport/fable_containers.h"
#pragma optimize("s",on)
namespace NUISystem { class CComponent; }

template std::vector<NUISystem::CComponent*>::iterator std::vector<NUISystem::CComponent*>::erase(iterator first, iterator last);
