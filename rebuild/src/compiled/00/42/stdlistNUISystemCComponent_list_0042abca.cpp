// std::list<NUISystem::CComponent*>::~list() -- STLport list destructor: unlink and free every node (0x0042A18F, the inline _List_base::clear), then free the sentinel node
// Leaf: ??1?$list@PAVCComponent@NUISystem@@V?$allocator@PAVCComponent@NUISystem@@@std@@@std@@QAE@XZ
// Leaf fix (sweep 3): STLport's allocator is a class, so the instantiation mangles V?$allocator (was U); the clear callee
//   binds the same way. Same bytes.
// Flags: /O2 /Oy /W3
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0042ABCA, 23 bytes (`ret`); 117 callers (NUISystem::CManager 0x0041E6D3 /
// 0x00423AF7.., CComponent destructors ..). Label correction (chunk 345 report): the manifest's "CFileInstaller::
// CActiveFile::OnReadFinished" is a propagated label shared by ~45 byte-identical list destructors (immediates masked:
// 0x00451763, 0x004DD118, 0x005B211F ..); this one belongs to the UI component list -- its clear callee 0x0042A18F frees
// 12-byte nodes with no element destructor (a pointer element) and its callers are the NUISystem::CManager / CComponent
// family that keeps list<CComponent*> children (retail CComponent children list, chunk 343/345).
// Callees: _List_base::clear @ 0x0042A18F (walks head->next freeing nodes with free 0x00BFEA14, then relinks the sentinel to
// itself; the manifest's "CGuiControlTreePane::SortTreeRecursively" is propagated), free @ 0x00BFEA14 for the sentinel.
// Layout: the list is STLport's one word, the sentinel-node pointer (_M_node); a node is {next, prev, CComponent* data}.
// Shape: size TU (`pop ecx` after the free); the sentinel is re-read after clear() and null-tested by
// allocator<T>::deallocate (`if (p) free(p)`, STLport's malloc allocator).
// NF42 (round 5): the local list / _List_node / allocator stand-ins replaced by the shared stlport/fable_containers.h
// (FABLE_RETAIL_STLP_LIST_DTOR_IN_LIST: the clear+free body in ~list itself, FableWin.pdb names ??1?$list; declaration-only
// _List_base::clear under FABLE_RETAIL_STLP_LIST_CLEAR_OUTOFLINE), so the
// clear callee binds STLport's ?clear@?$_List_base@..@std@@QAEXXZ (it was the list's own clear).
#pragma optimize("s", on)
#define FABLE_RETAIL_STLP_LIST_CLEAR_OUTOFLINE
#define FABLE_RETAIL_STLP_LIST_DTOR_IN_LIST
#include "stlport/fable_containers.h"
namespace NUISystem { class CComponent; }
template std::list<NUISystem::CComponent*, std::allocator<NUISystem::CComponent*> >::~list();
