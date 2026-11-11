// std::uninitialized_fill_n(CEngineLandscapeRenderer::CSubPatchSorterEntry* first, unsigned int n, CEngineLandscapeRenderer::CSubPatchSorterEntry const& x) -- STLport 4.6 _uninitialized.h: copy-construct n copies of x into raw storage at first (the __false_type helper inlined), return the end
// Leaf: ??$uninitialized_fill_n@PAUCSubPatchSorterEntry@CEngineLandscapeRenderer@@IU12@@std@@YIPAUCSubPatchSorterEntry@CEngineLandscapeRenderer@@PAU12@IABU12@@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B6B5E0, 40 bytes, /Gr `__fastcall` TU (first argument ecx, second edx, the rest on the stack), `ret 4`.
// Identity: caller 00b6c840 std::vector<CEngineLandscapeRenderer::CSubPatchSorterEntry, std::allocator<CEngineLandscapeRenderer::CSubPatchSorterEntry>>::_M_fill_insert; binder ??$uninitialized_fill_n@PAUCSubPatchSorterEntry@CEngineLandscapeRenderer@@IU12@@std@@YIPAUCSubPatchSorterEntry@CEngineLandscapeRenderer@@PAU12@IABU12@@Z
// Body: the STLport 4.6 template from the shared model rebuild/include/stlport/fable_containers.h, explicitly instantiated over CEngineLandscapeRenderer::CSubPatchSorterEntry.
#define FABLE_RETAIL_CENGINELANDSCAPERENDERER
#include "stlport/fable_containers.h"
#include "engine/CEngineLandscapeRenderer.h"
template CEngineLandscapeRenderer::CSubPatchSorterEntry* std::uninitialized_fill_n<CEngineLandscapeRenderer::CSubPatchSorterEntry*,unsigned int,CEngineLandscapeRenderer::CSubPatchSorterEntry >(CEngineLandscapeRenderer::CSubPatchSorterEntry*, unsigned int, CEngineLandscapeRenderer::CSubPatchSorterEntry const&);
