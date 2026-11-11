// std::__destroy_aux(CAbilityButtonGuiDef* first, CAbilityButtonGuiDef* last, std::__false_type const&) -- STLport 4.6 _construct.h: run the destructor of every element of [first, last) (the non-trivial-destructor path)
// Leaf: ??$__destroy_aux@PAVCAbilityButtonGuiDef@@@std@@YIXPAVCAbilityButtonGuiDef@@0ABU__false_type@0@@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x00450A58, 28 bytes, /Gr `__fastcall` TU (first argument ecx, second edx, the rest on the stack), `ret 4`.
// Identity: caller 004532e6 _global::StdList_Dtor_004532e6; caller 0046f06a std::vector<CAbilityButtonGuiDef>::operator=; binder ??$__destroy_aux@PAVCAbilityButtonGuiDef@@@std@@YIXPAVCAbilityButtonGuiDef@@0ABU__false_type@0@@Z
// Body: the STLport 4.6 template from the shared model rebuild/include/stlport/fable_containers.h, explicitly instantiated over CAbilityButtonGuiDef.
#pragma optimize("s",on)
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "stlport/fable_containers.h"
#include "engine/CAbilityButtonGuiDef.h"
template void std::__destroy_aux<CAbilityButtonGuiDef* >(CAbilityButtonGuiDef*, CAbilityButtonGuiDef*, std::__false_type const&);
