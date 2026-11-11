// std::__destroy_aux(CEnvironmentThemeDef* first, CEnvironmentThemeDef* last, std::__false_type const&) -- STLport 4.6 _construct.h: run the destructor of every element of [first, last) (the non-trivial-destructor path)
// Leaf: ??$__destroy_aux@PAVCEnvironmentThemeDef@@@std@@YIXPAVCEnvironmentThemeDef@@0ABU__false_type@0@@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x00450B1B, 31 bytes, /Gr `__fastcall` TU (first argument ecx, second edx, the rest on the stack), `ret 4`.
// Identity: caller 00472eda CEnvironmentThemeDefVector::operator=; caller 00473023 CEnvironmentThemeDefVector::clear; caller 00476f9b std::vector<CEnvironmentThemeDef>::erase; binder ??$__destroy_aux@PAVCEnvironmentThemeDef@@@std@@YIXPAVCEnvironmentThemeDef@@0ABU__false_type@0@@Z
// Body: the STLport 4.6 template from the shared model rebuild/include/stlport/fable_containers.h, explicitly instantiated over CEnvironmentThemeDef.
#pragma optimize("s",on)
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "stlport/fable_containers.h"
#include "engine/CEnvironmentThemeDef.h"
template void std::__destroy_aux<CEnvironmentThemeDef* >(CEnvironmentThemeDef*, CEnvironmentThemeDef*, std::__false_type const&);
