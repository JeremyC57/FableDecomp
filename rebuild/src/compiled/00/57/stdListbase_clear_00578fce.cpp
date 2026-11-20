#pragma optimize("s",on)
// std::_List_base<CSliderAnimator>::clear() -- STLport 4.6 _list.c: walk the circular list from the sentinel, destroy and free every node,
//   then point the sentinel back at itself
// Leaf: ?clear@?$_List_base@VCSliderAnimator@@V?$allocator@VCSliderAnimator@@@std@@@std@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00578FCE, 39 bytes (size-optimised TU; free 0x00BFEA14 per node, CSliderAnimator's destructor is trivial).
// Identity (descriptive-by-library: both PDBs are Dinkumware builds, where FableWin.pdb has list<CSliderAnimator>::clear): STLport 4.6
//   defines clear() in _List_base, and list<T>::clear() is the inline `_Base::clear()`, so the out-of-line COMDAT is
//   _List_base<CSliderAnimator>::clear. Its callers bind exactly that: 0x00577216 / 0x00577AC0 (CTCInventoryExperience
//   `Animators.clear()` on the shared header) and the ~_List_base 0x005792DA (whose own caller 0x0057846F binds
//   ??1?$_List_base@VCSliderAnimator@@...). The queue's list<CSliderAnimator>::clear spelling is FableWin's Dinkumware member.
// NF113 (PRIORITY q4773): was the stand-in List::Clear (mislabelled CGuiControlTreePane::SortTreeRecursively) over local Node / List
//   models; now the shared STLport text (stlport/fable_containers.h) on engine/CSliderAnimator.h, emitted by explicit instantiation
//   (the 0x00566E10 / 0x005F900A recipes).
#define FABLE_RETAIL_STLP_LIST
#include "stlport/fable_containers.h"
#include "engine/CSliderAnimator.h"
template void std::_List_base<CSliderAnimator, std::allocator<CSliderAnimator> >::clear();
