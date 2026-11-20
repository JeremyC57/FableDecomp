// std::list<CSliderAnimator>::push_back(const CSliderAnimator& x) -- STLport 4.6 `void push_back(const _Tp& __x) { insert(end(), __x); }`:
//   the end() iterator (the sentinel node pointer) by value and the element by reference into the out-of-line insert, whose returned
//   iterator goes through a hidden pointer (the dead argument slot)
// Leaf: ?push_back@?$list@VCSliderAnimator@@V?$allocator@VCSliderAnimator@@@std@@@std@@QAEXABVCSliderAnimator@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0057955A, 26 bytes (`push ebp ; mov ebp,esp ; push [ebp+8] ; mov edx,[ecx] ; push ecx ;
//   mov eax,esp ; mov [eax],edx ; lea eax,[ebp+8] ; push eax ; call 0x0057929E ; pop ebp ; ret 4`), a size-optimised TU with frame pointers.
// Identity: STLport 4.6 list<CSliderAnimator>::push_back (descriptive-by-library: both PDBs are Dinkumware builds; FableWin.pdb's
//   CSliderAnimator list instances confirm the element type); the landed callers 0x00577216 / 0x00577AC0 (CTCInventoryExperience
//   `Animators.push_back(general)` on the shared header) already bind this spelling. Callee: insert(iterator, const T&) 0x0057929E, out of
//   line in this TU (FABLE_RETAIL_STLP_LIST_INSERT_OUTOFLINE, the chunk-892 0x005A887D form).
// NF113 (PRIORITY q4772): was the stand-in ListWrapper_0057955a::ListPushFrontWrapper_0057955a(unsigned long) over a local iterator
//   model; now the shared STLport list (stlport/fable_containers.h) on engine/CSliderAnimator.h, emitted by explicit instantiation.
#pragma optimize("s",on)
#define FABLE_RETAIL_STLP_LIST_INSERT_OUTOFLINE
#include "stlport/fable_containers.h"
#include "engine/CSliderAnimator.h"
#pragma optimize("y",off)
template void std::list<CSliderAnimator>::push_back(const CSliderAnimator& x);
#pragma optimize("y",on)
