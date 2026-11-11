// std::__destroy_aux(CCountedPointer<NUISystem::CAction>* first, CCountedPointer<NUISystem::CAction>* last, std::__false_type const&) -- STLport 4.6 _construct.h: run the destructor of every element of [first, last) (the non-trivial-destructor path)
// Leaf: ??$__destroy_aux@PAV?$CCountedPointer@VCAction@NUISystem@@@@@std@@YIXPAV?$CCountedPointer@VCAction@NUISystem@@@@0ABU__false_type@0@@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x0054A1C0, 76 bytes, /Gr `__fastcall` TU (first argument ecx, second edx, the rest on the stack), `ret 4`.
// Identity: caller 00549a00 source (NUISystemCTextSlider_CTextSlider_00549a00.cpp) names CCountedPointer<NUISystem::CAction>; caller 0054a2a0 source (stdvector_Minsertoverflow_0054a2a0.cpp) names CCountedPointer<NUISystem::CAction>; binder ??$__destroy_aux@PAV?$CCountedPointer@VCAction@NUISystem@@@@@std@@YIXPAV?$CCountedPointer@VCAction@NUISystem@@@@0ABU__false_type@0@@Z
// Body: the STLport 4.6 template from the shared model rebuild/include/stlport/fable_containers.h, explicitly instantiated over CCountedPointer<NUISystem::CAction>.
#include "stlport/fable_containers.h"
#include "engine/CCountedPointer.h"
#include "engine/NUISystem_CAction.h"
template void std::__destroy_aux<CCountedPointer<NUISystem::CAction>* >(CCountedPointer<NUISystem::CAction>*, CCountedPointer<NUISystem::CAction>*, std::__false_type const&);
