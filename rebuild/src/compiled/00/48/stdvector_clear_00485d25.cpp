// std::vector<NUISystem::CComponent*>::clear() -- STLport 4.6 `erase(begin(), end())`, the range erase called out of line
// Leaf: ?clear@?$vector@PAVCComponent@NUISystem@@V?$allocator@PAVCComponent@NUISystem@@@std@@@std@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00485D25, 11 bytes: `push dword ptr [ecx+4] ; push dword ptr [ecx] ; call 0x00485AA7 ; ret`
//   (size-optimised TU). 0x00485AA7 is vector<NUISystem::CComponent*>::erase(iterator, iterator) (catalog; bytes: __copy of the tail,
//   finish updated, `ret 8`).
// Name fix (NF195, 2026-11-20, q6331 / q6339): was the generated descriptive CClass_00485d25::ApplyPair_00485d25 (members +0 / +4 passed
//   to an invented Apply). STLport spelling (descriptive-by-library: no PDB holds STL instances); the only landed caller 0x005E78F0
//   (CTCQuestCompletionUI's component vector) already binds this clear spelling. The erase it calls is now bound under the STLport
//   spelling too (count-neutral: the stand-in ?Apply@CClass_00485d25@@ leaves the address); the erase row 0x00485AA7 itself is a
//   July model (CGuiControlListBox::SelectEntryFromData, no Leaf) left for its own pass.
#define FABLE_RETAIL_STLP_VECTOR_ERASE_RANGE_OUTOFLINE
#include "stlport/fable_containers.h"
#pragma optimize("s",on)
namespace NUISystem { class CComponent; }

template void std::vector<NUISystem::CComponent*>::clear();
