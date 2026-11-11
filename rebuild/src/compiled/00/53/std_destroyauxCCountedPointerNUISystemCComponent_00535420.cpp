// std::__destroy_aux(CCountedPointer<NUISystem::CComponent>* first, CCountedPointer<NUISystem::CComponent>* last, std::__false_type const&) -- STLport 4.6 _construct.h: run the destructor of every element of [first, last) (the non-trivial-destructor path)
// Leaf: ??$__destroy_aux@PAV?$CCountedPointer@VCComponent@NUISystem@@@@@std@@YIXPAV?$CCountedPointer@VCComponent@NUISystem@@@@0ABU__false_type@0@@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x00535420, 76 bytes, /Gr `__fastcall` TU (first argument ecx, second edx, the rest on the stack), `ret 4`.
// Identity: caller 00535900 std::vector<CCountedPointer<NUISystem::CComponent>, std::allocator<CCountedPointer<NUISystem::CComponent>>>::~vector; binder ??$__destroy_aux@PAV?$CCountedPointer@VCComponent@NUISystem@@@@@std@@YIXPAV?$CCountedPointer@VCComponent@NUISystem@@@@0ABU__false_type@0@@Z
// Body: the STLport 4.6 template from the shared model rebuild/include/stlport/fable_containers.h, explicitly instantiated over CCountedPointer<NUISystem::CComponent>.
#include "stlport/fable_containers.h"
#include "engine/CCountedPointer.h"
#include "engine/NUISystem_CComponent.h"
template void std::__destroy_aux<CCountedPointer<NUISystem::CComponent>* >(CCountedPointer<NUISystem::CComponent>*, CCountedPointer<NUISystem::CComponent>*, std::__false_type const&);
