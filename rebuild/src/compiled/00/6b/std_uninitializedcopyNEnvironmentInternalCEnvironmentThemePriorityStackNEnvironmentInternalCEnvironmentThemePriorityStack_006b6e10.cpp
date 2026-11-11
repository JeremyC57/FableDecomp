// std::__uninitialized_copy(NEnvironmentInternal::CEnvironmentThemePriorityStack* first, NEnvironmentInternal::CEnvironmentThemePriorityStack* last, NEnvironmentInternal::CEnvironmentThemePriorityStack* result, std::__false_type const&) -- STLport 4.6 _uninitialized.h: copy-construct [first, last) into raw storage at result (placement new per element), return the end
// Leaf: ??$__uninitialized_copy@PAVCEnvironmentThemePriorityStack@NEnvironmentInternal@@PAV12@@std@@YIPAVCEnvironmentThemePriorityStack@NEnvironmentInternal@@PAV12@00ABU__false_type@0@@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x006B6E10, 46 bytes, /Gr `__fastcall` TU (first argument ecx, second edx, the rest on the stack), `ret 8`.
// Identity: caller 006b7130 std::vector<NEnvironmentInternal::CEnvironmentThemePriorityStack, std::allocator<NEnvironmentInternal::CEnvironmentThemePriorityStack>>::_M_insert_overflow; callee 006b6840 std::vector::vector; binder ??$__uninitialized_copy@PAVCEnvironmentThemePriorityStack@NEnvironmentInternal@@PAV12@@std@@YIPAVCEnvironmentThemePriorityStack@NEnvironmentInternal@@PAV12@00ABU__false_type@0@@Z
// Body: the STLport 4.6 template from the shared model rebuild/include/stlport/fable_containers.h, explicitly instantiated over NEnvironmentInternal::CEnvironmentThemePriorityStack.
#include "stlport/fable_containers.h"
#include "engine/CEnvironmentThemePriorityStack.h"
template NEnvironmentInternal::CEnvironmentThemePriorityStack* std::__uninitialized_copy<NEnvironmentInternal::CEnvironmentThemePriorityStack*,NEnvironmentInternal::CEnvironmentThemePriorityStack* >(NEnvironmentInternal::CEnvironmentThemePriorityStack*, NEnvironmentInternal::CEnvironmentThemePriorityStack*, NEnvironmentInternal::CEnvironmentThemePriorityStack*, std::__false_type const&);
