// std::_List_base::clear() -- STLport 4.6 _list.c clear(): free every node after the sentinel, then relink the sentinel to itself (std::list<NUISystem::CList*>)
// Leaf: ?clear@?$_List_base@PAVCList@NUISystem@@V?$allocator@PAVCList@NUISystem@@@std@@@std@@QAEXXZ
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004855A3, 39 bytes.
// Name (agentNF23, name-fix queue land1011da, agentFAM07 proof): every caller passes ecx = 0x013B8A98, the static
//   NUISystem::CList::GPreviousSelected (Ego_r.pdb dynamic initializer / atexit destructor of 'NUISystem::CList::GPreviousSelected';
//   FableWin.pdb ?GetPreviousSelected@CList@NUISystem@@SIAAV?$list@PAVCList@NUISystem@@... gives the type), and
//   NUISystem::CList::ClearPreviousSelected 0x0048573D (FableWin.pdb ?ClearPreviousSelected@CList@NUISystem@@SIXXZ) is
//   `mov ecx, offset GPreviousSelected ; jmp 0x004855A3`: list<NUISystem::CList*>::clear(), STLport's _List_base<..>::clear (public).
//   The node values are pointers (no destructor call); nodes freed with free 0x00BFEA14. Was the stand-in List::Clear.
// Model: the shared STLport 4.6 list (stlport/fable_containers.h); the explicit instantiation emits clear() as its own COMDAT.
#pragma optimize("s",on)                                   // size-optimised TU (as the landed row: /Os bytes)
#include "stlport/fable_containers.h"
namespace NUISystem { class CList; }
template void std::_List_base<NUISystem::CList*, std::allocator<NUISystem::CList*> >::clear();
