// NHeroInformationScreens::CManager::InitialiseScrollBarIndices(long count) -- reset ScrollBarIndices to 0, 1, ..., count-1
// Leaf: ?InitialiseScrollBarIndices@CManager@NHeroInformationScreens@@QAEXJ@Z
// Retail: Fable.exe @ 0x0066EA85, 48 bytes, `ret 4`; Ego_r.pdb + FableWin.pdb ?InitialiseScrollBarIndices@CManager@NHeroInformationScreens@@QAEXJ@Z;
// four callers in the NHeroInformationScreens::CManager block (0x0066EB04 .. 0x0066EC04). Callees are the STLport
// vector<long> members erase(first, last) (0x00440590, via clear()) and resize(n) (0x004433F0, which forwards to
// resize(n, long())); then ScrollBarIndices[i] = i, re-reading _M_start every pass.
// Opt: size TU (unaligned entry; clear() is inlined but erase/resize stay calls).
// Name fix (agentNF94, q11238): moved off the row-local NHeroInformationScreens::CVector<T> / CManager models onto the shared
//   engine/NHeroInformationScreens_CManager.h (_LAYOUT: ScrollBarIndices +0x1c, a std::vector<long>; overlay opt-in
//   _INITIALISESCROLLBARINDICES) and stlport/fable_containers.h, so the callees bind the STLport spellings the landed rows use:
//   ?erase@?$vector@JV?$allocator@J@std@@@std@@QAEPAJPAJ0@Z (0x00440590) and ?resize@?$vector@JV?$allocator@J@std@@@std@@QAEXI@Z
//   (0x004433F0, the catalog's std::vector<long>::resize and the landed 0x0064E0A1 binding). Both are out of line in this TU
//   (FABLE_RETAIL_STLP_VECTOR_ERASE_RANGE_OUTOFLINE, FABLE_RETAIL_STLP_VECTOR_RESIZE_ONE_DECL).
#pragma optimize("s", on)
#define FABLE_RETAIL_STLP_VECTOR_ERASE_RANGE_OUTOFLINE
#define FABLE_RETAIL_STLP_VECTOR_RESIZE_ONE_DECL
#define FABLE_RETAIL_NHEROINFORMATIONSCREENS_CMANAGER
#define FABLE_RETAIL_NHEROINFORMATIONSCREENS_CMANAGER_LAYOUT
#define FABLE_RETAIL_NHEROINFORMATIONSCREENS_CMANAGER_INITIALISESCROLLBARINDICES
#include "stlport/fable_containers.h"
#include "engine/NHeroInformationScreens_CManager.h"
namespace NHeroInformationScreens {
void CManager::InitialiseScrollBarIndices(long count)
{
    ScrollBarIndices.clear();
    ScrollBarIndices.resize(count);
    for (long i = 0; i < count; ++i)
        ScrollBarIndices[i] = i;
}
}
