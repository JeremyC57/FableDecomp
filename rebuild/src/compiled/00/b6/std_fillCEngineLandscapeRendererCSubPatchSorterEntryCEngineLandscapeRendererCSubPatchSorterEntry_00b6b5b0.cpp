// std::fill(CEngineLandscapeRenderer::CSubPatchSorterEntry* first, CEngineLandscapeRenderer::CSubPatchSorterEntry* last, CEngineLandscapeRenderer::CSubPatchSorterEntry const& x) -- STLport 4.6 _algobase.h: assign x to every element of [first, last)
// Leaf: ??$fill@PAUCSubPatchSorterEntry@CEngineLandscapeRenderer@@U12@@std@@YIXPAUCSubPatchSorterEntry@CEngineLandscapeRenderer@@0ABU12@@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B6B5B0, 37 bytes, /Gr `__fastcall` TU (first argument ecx, second edx, the rest on the stack), `ret 4`.
// Identity: caller 00b6c840 std::vector<CEngineLandscapeRenderer::CSubPatchSorterEntry, std::allocator<CEngineLandscapeRenderer::CSubPatchSorterEntry>>::_M_fill_insert; caller 00b6c840 std::vector<CEngineLandscapeRenderer::CSubPatchSorterEntry, std::allocator<CEngineLandscapeRenderer::CSubPatchSorterEntry>>::_M_fill_insert; binder ??$fill@PAUCSubPatchSorterEntry@CEngineLandscapeRenderer@@U12@@std@@YIXPAUCSubPatchSorterEntry@CEngineLandscapeRenderer@@0ABU12@@Z
// Body: the STLport 4.6 template from the shared model rebuild/include/stlport/fable_containers.h, explicitly instantiated over CEngineLandscapeRenderer::CSubPatchSorterEntry.
#define FABLE_RETAIL_CENGINELANDSCAPERENDERER
#include "stlport/fable_containers.h"
#include "engine/CEngineLandscapeRenderer.h"
template void std::fill<CEngineLandscapeRenderer::CSubPatchSorterEntry*,CEngineLandscapeRenderer::CSubPatchSorterEntry >(CEngineLandscapeRenderer::CSubPatchSorterEntry*, CEngineLandscapeRenderer::CSubPatchSorterEntry*, CEngineLandscapeRenderer::CSubPatchSorterEntry const&);
