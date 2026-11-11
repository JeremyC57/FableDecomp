// std::__destroy_aux(CCountedPointer<CCombatSequenceBase>* first, CCountedPointer<CCombatSequenceBase>* last, std::__false_type const&) -- STLport 4.6 _construct.h: run the destructor of every element of [first, last) (the non-trivial-destructor path)
// Leaf: ??$__destroy_aux@PAV?$CCountedPointer@VCCombatSequenceBase@@@@@std@@YIXPAV?$CCountedPointer@VCCombatSequenceBase@@@@0ABU__false_type@0@@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x006F2300, 76 bytes, /Gr `__fastcall` TU (first argument ecx, second edx, the rest on the stack), `ret 4`.
// Identity: caller 006ed270 source (CCombatManager_RegisterAttackedCombatant_006ed270.cpp) names CCountedPointer<CCombatSequenceBase>; caller 006ed510 source (CCombatManager_DeregisterAttackingCombatant_006ed510.cpp) names CCountedPointer<CCombatSequenceBase>; binder ??$__destroy_aux@PAV?$CCountedPointer@VCCombatSequenceBase@@@@@std@@YIXPAV?$CCountedPointer@VCCombatSequenceBase@@@@0ABU__false_type@0@@Z
// Body: the STLport 4.6 template from the shared model rebuild/include/stlport/fable_containers.h, explicitly instantiated over CCountedPointer<CCombatSequenceBase>.
#include "stlport/fable_containers.h"
#include "engine/CCountedPointer.h"
class CCombatSequenceBase;
template void std::__destroy_aux<CCountedPointer<CCombatSequenceBase>* >(CCountedPointer<CCombatSequenceBase>*, CCountedPointer<CCombatSequenceBase>*, std::__false_type const&);
