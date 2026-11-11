// std::__uninitialized_copy(NEnvironmentInternal::CBlendedEnvironmentTheme const* first, NEnvironmentInternal::CBlendedEnvironmentTheme const* last, NEnvironmentInternal::CBlendedEnvironmentTheme* result, std::__false_type const&) -- STLport 4.6 _uninitialized.h: copy-construct [first, last) into raw storage at result (placement new per element), return the end
// Leaf: ??$__uninitialized_copy@PBVCBlendedEnvironmentTheme@NEnvironmentInternal@@PAV12@@std@@YIPAVCBlendedEnvironmentTheme@NEnvironmentInternal@@PBV12@0PAV12@ABU__false_type@0@@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x00889880, 52 bytes, /Gr `__fastcall` TU (first argument ecx, second edx, the rest on the stack), `ret 8`.
// Identity: callee 008894a0 NEnvironmentInternal::CBlendedEnvironmentTheme::CBlendedEnvironmentTheme; binder ??$__uninitialized_copy@PBVCBlendedEnvironmentTheme@NEnvironmentInternal@@PAV12@@std@@YIPAVCBlendedEnvironmentTheme@NEnvironmentInternal@@PBV12@0PAV12@ABU__false_type@0@@Z
// Body: the STLport 4.6 template from the shared model rebuild/include/stlport/fable_containers.h, explicitly instantiated over NEnvironmentInternal::CBlendedEnvironmentTheme.
#include "stlport/fable_containers.h"
#include "engine/CBlendedEnvironmentTheme.h"
template NEnvironmentInternal::CBlendedEnvironmentTheme* std::__uninitialized_copy<NEnvironmentInternal::CBlendedEnvironmentTheme const*,NEnvironmentInternal::CBlendedEnvironmentTheme* >(NEnvironmentInternal::CBlendedEnvironmentTheme const*, NEnvironmentInternal::CBlendedEnvironmentTheme const*, NEnvironmentInternal::CBlendedEnvironmentTheme*, std::__false_type const&);
