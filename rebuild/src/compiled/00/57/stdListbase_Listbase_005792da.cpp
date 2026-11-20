// std::_List_base<CSliderAnimator>::~_List_base() -- STLport list destructor: clear the nodes (out-of-line _List_base::clear @ 0x00578FCE) then free the sentinel node
// Leaf: ??1?$_List_base@VCSliderAnimator@@V?$allocator@VCSliderAnimator@@@std@@@std@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x005792DA, 23 bytes: `mov esi,ecx ; call clear (0x00578FCE) ; if (sentinel) free(sentinel)` (size-optimised
//   TU: pop ecx).
// Identity (descriptive-by-library: STLport 4.6; both PDBs are Dinkumware builds): the only landed caller 0x0057846F binds
//   ??1?$_List_base@VCSliderAnimator@@V?$allocator@VCSliderAnimator@@@std@@@std@@QAE@XZ, and the clear it calls, 0x00578FCE, is the
//   list<CSliderAnimator> instance the CTCInventoryExperience rows 0x00577216 / 0x00577AC0 call. Was the descriptive placeholder
//   std::list_T_005792da (renamed from CFileInstaller::CActiveFile::OnReadFinished, fistp pass 2026-10-24: "the node type T is not
//   recoverable from the bytes"; the caller now proves it).
// NF113 (PRIORITY q4773, binder of 0x00578FCE): re-authored on the shared STLport text (the 0x005F900A recipe:
//   FABLE_RETAIL_STLP_LIST_CLEAR_OUTOFLINE, explicit destructor instantiation) on engine/CSliderAnimator.h.
#pragma optimize("s", on)
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_STLP_LIST_CLEAR_OUTOFLINE
#include "stlport/fable_containers.h"
#include "engine/CSliderAnimator.h"
template std::_List_base<CSliderAnimator, std::allocator<CSliderAnimator> >::~_List_base();
