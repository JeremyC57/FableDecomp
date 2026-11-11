// std::__uninitialized_copy(CMinimapData::MinimapTheme const* first, CMinimapData::MinimapTheme const* last, CMinimapData::MinimapTheme* result, std::__false_type const&) -- STLport 4.6 _uninitialized.h: copy-construct [first, last) into raw storage at result (placement new per element), return the end
// Leaf: ??$__uninitialized_copy@PBW4MinimapTheme@CMinimapData@@PAW412@@std@@YIPAW4MinimapTheme@CMinimapData@@PBW412@0PAW412@ABU__false_type@0@@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x0082C420, 38 bytes, /Gr `__fastcall` TU (first argument ecx, second edx, the rest on the stack), `ret 8`.
// Identity: caller 0082c980 source (vector_operator_0082c980.cpp) names CMinimapData::MinimapTheme; binder ??$__uninitialized_copy@PBW4MinimapTheme@CMinimapData@@PAW412@@std@@YIPAW4MinimapTheme@CMinimapData@@PBW412@0PAW412@ABU__false_type@0@@Z
// Body: the STLport 4.6 template from the shared model rebuild/include/stlport/fable_containers.h, explicitly instantiated over CMinimapData::MinimapTheme.
#include "stlport/fable_containers.h"
class CMinimapData {
public:
enum MinimapTheme { };
};
template CMinimapData::MinimapTheme* std::__uninitialized_copy<CMinimapData::MinimapTheme const*,CMinimapData::MinimapTheme* >(CMinimapData::MinimapTheme const*, CMinimapData::MinimapTheme const*, CMinimapData::MinimapTheme*, std::__false_type const&);
