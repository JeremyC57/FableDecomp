// std::__uninitialized_copy(CEngineLandscapeRenderer::CSubPatchSorterEntry* first, CEngineLandscapeRenderer::CSubPatchSorterEntry* last, CEngineLandscapeRenderer::CSubPatchSorterEntry* result, std::__false_type const&) -- STLport 4.6 _uninitialized.h: copy-construct [first, last) into raw storage at result (placement new per element), return the end
// Leaf: ??$__uninitialized_copy@PAUCSubPatchSorterEntry@CEngineLandscapeRenderer@@PAU12@@std@@YIPAUCSubPatchSorterEntry@CEngineLandscapeRenderer@@PAU12@00ABU__false_type@0@@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B6B4A0, 44 bytes, /Gr `__fastcall` TU (first argument ecx, second edx, the rest on the stack), `ret 8`.
// Identity: caller 00b6c840 std::vector<CEngineLandscapeRenderer::CSubPatchSorterEntry, std::allocator<CEngineLandscapeRenderer::CSubPatchSorterEntry>>::_M_fill_insert; caller 00b6c840 std::vector<CEngineLandscapeRenderer::CSubPatchSorterEntry, std::allocator<CEngineLandscapeRenderer::CSubPatchSorterEntry>>::_M_fill_insert; binder ??$__uninitialized_copy@PAUCSubPatchSorterEntry@CEngineLandscapeRenderer@@PAU12@@std@@YIPAUCSubPatchSorterEntry@CEngineLandscapeRenderer@@PAU12@00ABU__false_type@0@@Z
// Body: the STLport 4.6 template from the shared model rebuild/include/stlport/fable_containers.h, explicitly instantiated over CEngineLandscapeRenderer::CSubPatchSorterEntry.
#define FABLE_RETAIL_CENGINELANDSCAPERENDERER
#include "stlport/fable_containers.h"
#include "engine/CEngineLandscapeRenderer.h"
template CEngineLandscapeRenderer::CSubPatchSorterEntry* std::__uninitialized_copy<CEngineLandscapeRenderer::CSubPatchSorterEntry*,CEngineLandscapeRenderer::CSubPatchSorterEntry* >(CEngineLandscapeRenderer::CSubPatchSorterEntry*, CEngineLandscapeRenderer::CSubPatchSorterEntry*, CEngineLandscapeRenderer::CSubPatchSorterEntry*, std::__false_type const&);
