// std::__uninitialized_fill_n(NEnvironmentInternal::CEnvironmentThemePriorityStackEntry* first, unsigned int n, NEnvironmentInternal::CEnvironmentThemePriorityStackEntry const& x, std::__false_type const&) -- STLport 4.6 _uninitialized.h: copy-construct n copies of x into raw storage at first, return the end
// Leaf: ??$__uninitialized_fill_n@PAVCEnvironmentThemePriorityStackEntry@NEnvironmentInternal@@IV12@@std@@YIPAVCEnvironmentThemePriorityStackEntry@NEnvironmentInternal@@PAV12@IABV12@ABU__false_type@0@@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x00889230, 86 bytes, /Gr `__fastcall` TU (first argument ecx, second edx, the rest on the stack), `ret 8`.
// Identity: callee 006b58f0 std::vector::vector; binder ??$__uninitialized_fill_n@PAVCEnvironmentThemePriorityStackEntry@NEnvironmentInternal@@IV12@@std@@YIPAVCEnvironmentThemePriorityStackEntry@NEnvironmentInternal@@PAV12@IABV12@ABU__false_type@0@@Z
// Body: the STLport 4.6 template from the shared model rebuild/include/stlport/fable_containers.h, explicitly instantiated over NEnvironmentInternal::CEnvironmentThemePriorityStackEntry.
#include "stlport/fable_containers.h"
#include "engine/CEnvironmentThemePriorityStackEntry.h"
template NEnvironmentInternal::CEnvironmentThemePriorityStackEntry* std::__uninitialized_fill_n<NEnvironmentInternal::CEnvironmentThemePriorityStackEntry*,unsigned int,NEnvironmentInternal::CEnvironmentThemePriorityStackEntry >(NEnvironmentInternal::CEnvironmentThemePriorityStackEntry*, unsigned int, NEnvironmentInternal::CEnvironmentThemePriorityStackEntry const&, std::__false_type const&);
