// std::__uninitialized_fill_n(NEnvironmentInternal::CEnvironmentThemePriorityStackEntry::CThemeWeight* first, unsigned int n, NEnvironmentInternal::CEnvironmentThemePriorityStackEntry::CThemeWeight const& x, std::__false_type const&) -- STLport 4.6 _uninitialized.h: copy-construct n copies of x into raw storage at first, return the end
// Leaf: ??$__uninitialized_fill_n@PAUCThemeWeight@CEnvironmentThemePriorityStackEntry@NEnvironmentInternal@@IU123@@std@@YIPAUCThemeWeight@CEnvironmentThemePriorityStackEntry@NEnvironmentInternal@@PAU123@IABU123@ABU__false_type@0@@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x00885CE0, 52 bytes, /Gr `__fastcall` TU (first argument ecx, second edx, the rest on the stack), `ret 8`.
// Identity: caller 00889010 std::vector<NEnvironmentInternal::CEnvironmentThemePriorityStackEntry::CThemeWeight, std::allocator<NEnvironmentInternal::CEnvironmentThemePriorityStackEntry::CThemeWeight>>::_M_insert_overflow; binder ??$__uninitialized_fill_n@PAUCThemeWeight@CEnvironmentThemePriorityStackEntry@NEnvironmentInternal@@IU123@@std@@YIPAUCThemeWeight@CEnvironmentThemePriorityStackEntry@NEnvironmentInternal@@PAU123@IABU123@ABU__false_type@0@@Z
// Body: the STLport 4.6 template from the shared model rebuild/include/stlport/fable_containers.h, explicitly instantiated over NEnvironmentInternal::CEnvironmentThemePriorityStackEntry::CThemeWeight.
#include "stlport/fable_containers.h"
namespace NEnvironmentInternal {
class CEnvironmentThemePriorityStackEntry {
public:
struct CThemeWeight { long m_0; long m_4; long m_8;
};
};
}
template NEnvironmentInternal::CEnvironmentThemePriorityStackEntry::CThemeWeight* std::__uninitialized_fill_n<NEnvironmentInternal::CEnvironmentThemePriorityStackEntry::CThemeWeight*,unsigned int,NEnvironmentInternal::CEnvironmentThemePriorityStackEntry::CThemeWeight >(NEnvironmentInternal::CEnvironmentThemePriorityStackEntry::CThemeWeight*, unsigned int, NEnvironmentInternal::CEnvironmentThemePriorityStackEntry::CThemeWeight const&, std::__false_type const&);
