// std::_List_base<NUISystem::CComponent*>::~_List_base() -- STLport list base destructor: unlink and free every node (0x0042A18F, _List_base::clear out of line), then free the sentinel node
// Leaf: ??1?$_List_base@PAVCComponent@NUISystem@@V?$allocator@PAVCComponent@NUISystem@@@std@@@std@@QAE@XZ
// Name fix (NF239, 2026-11-22, CODEXP27 L18): the base-class destructor, not list's own. Two sources: retail 0x0042B6D2 is the
//   size-optimised list<CComponent*>::~list() COMDAT, a 5-byte `jmp 0x0042ABCA` (STLport 4.6 _list.h `~list() {}`, the clear + free
//   in ~_List_base), next to the list(const allocator&) COMDAT 0x0042B6A6 that calls _List_base's constructor 0x0042ABAF; 34 landed
//   binders (e.g. 0x0041E6D3, 0x00570332, 0x005F11A9) already bind this spelling. The FABLE_RETAIL_STLP_LIST_DTOR_IN_LIST rationale
//   ("FableWin.pdb names ??1?$list, no ??1?$_List_base") reads a Dinkumware PDB (no _List_base exists there at all), so it does
//   not arbitrate STLport layering. Was ??1?$list@..@QAE@XZ; 31 landed binders still spell that (the list<CComponent*> family pass).
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
// (declaration-only _List_base::clear under FABLE_RETAIL_STLP_LIST_CLEAR_OUTOFLINE), so the clear callee binds STLport's
// ?clear@?$_List_base@..@std@@QAEXXZ (it was the list's own clear). NF239: FABLE_RETAIL_STLP_LIST_DTOR_IN_LIST dropped; the row
// instantiates _List_base's own destructor (see the name fix above).
#pragma optimize("s", on)
#define FABLE_RETAIL_STLP_LIST_CLEAR_OUTOFLINE
#include "stlport/fable_containers.h"
namespace NUISystem { class CComponent; }
template std::_List_base<NUISystem::CComponent*, std::allocator<NUISystem::CComponent*> >::~_List_base();
