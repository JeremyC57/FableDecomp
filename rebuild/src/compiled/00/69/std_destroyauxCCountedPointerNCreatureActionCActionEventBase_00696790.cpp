// std::__destroy_aux(CCountedPointer<NCreatureAction::CActionEventBase>* first, CCountedPointer<NCreatureAction::CActionEventBase>* last, std::__false_type const&) -- STLport 4.6 _construct.h: run the destructor of every element of [first, last) (the non-trivial-destructor path)
// Leaf: ??$__destroy_aux@PAV?$CCountedPointer@VCActionEventBase@NCreatureAction@@@@@std@@YIXPAV?$CCountedPointer@VCActionEventBase@NCreatureAction@@@@0ABU__false_type@0@@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x00696790, 76 bytes, /Gr `__fastcall` TU (first argument ecx, second edx, the rest on the stack), `ret 4`.
// Identity: caller 00693ef0 source (CCreatureActionBase_CCreatureActionBase_00693ef0.cpp) names CCountedPointer<NCreatureAction::CActionEventBase>; caller 00696f00 source (stdvector_Minsertoverflow_00696f00.cpp) names CCountedPointer<NCreatureAction::CActionEventBase>; binder ??$__destroy_aux@PAV?$CCountedPointer@VCActionEventBase@NCreatureAction@@@@@std@@YIXPAV?$CCountedPointer@VCActionEventBase@NCreatureAction@@@@0ABU__false_type@0@@Z
// Body: the STLport 4.6 template from the shared model rebuild/include/stlport/fable_containers.h, explicitly instantiated over CCountedPointer<NCreatureAction::CActionEventBase>.
#include "stlport/fable_containers.h"
#include "engine/CCountedPointer.h"
namespace NCreatureAction {
class CActionEventBase;
}
template void std::__destroy_aux<CCountedPointer<NCreatureAction::CActionEventBase>* >(CCountedPointer<NCreatureAction::CActionEventBase>*, CCountedPointer<NCreatureAction::CActionEventBase>*, std::__false_type const&);
