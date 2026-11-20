// std::list<NUISystem::CList*>::~list() -- STLport list destructor: clear the nodes (out-of-line _List_base::clear @ 0x004855A3) then free the sentinel node
// Leaf: ??1?$list@PAVCList@NUISystem@@V?$allocator@PAVCList@NUISystem@@@std@@@std@@QAE@XZ
// Renamed (fistp pass, 2026-10-24) from CFileInstaller::CActiveFile::OnReadFinished @ 00/53/CActiveFile_OnReadFinished_0053c9bc.cpp: 23/25-byte STLport list destructor: `mov esi,ecx ; call clear (0x004855A3) ; if (sentinel) free(sentinel)`; (the label CFileInstaller::CActiveFile::OnReadFinished was propagated across 165 byte-twins) (size-optimised TU: pop ecx)
// Retail: Fable.exe (TLC, VC7.1) @ 0x0053C9BC, 23 bytes.
// Name fix (NF202, 2026-11-20, q12208 / q12212): the element type is proven: the only reference is the atexit thunk 0x01228E44
//   (`mov ecx,0x013B8A98 ; jmp`) of NUISystem::CList::GPreviousSelected, a list<NUISystem::CList*> (FableWin accessor), and the clear
//   callee 0x004855A3 is std::_List_base<NUISystem::CList*>::clear (catalog). FableWin.pdb 0x021A8566 names
//   ??1?$list@PAVCList@NUISystem@@V?$allocator@PAVCList@NUISystem@@@std@@@std@@QAE@XZ. The local list_T_0053c9bc stand-in is replaced by
//   stlport/fable_containers.h (FABLE_RETAIL_STLP_LIST_DTOR_IN_LIST: the clear+free body in ~list itself, as row 0x0042ABCA;
//   FABLE_RETAIL_STLP_LIST_CLEAR_OUTOFLINE: _List_base::clear declared only). Was ??1list_T_0053c9bc@std@@QAE@XZ.
#pragma optimize("s", on)
#define FABLE_RETAIL_STLP_LIST_CLEAR_OUTOFLINE
#define FABLE_RETAIL_STLP_LIST_DTOR_IN_LIST
#include "stlport/fable_containers.h"
namespace NUISystem { class CList; }
template std::list<NUISystem::CList*, std::allocator<NUISystem::CList*> >::~list();
